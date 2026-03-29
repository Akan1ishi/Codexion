#!/usr/bin/python3

import argparse
import io
import subprocess
import sys
import typing

RED            = "\033[31m"
GREEN          = "\033[32m"
YELLOW         = "\033[33m"
MAGENTA        = "\033[35m"
CYAN           = "\033[36m"
BRIGHT_GREEN   = "\033[92m"
RESET          = "\033[0m"


class Action(typing.NamedTuple):
    timestamp: int
    action:    str

    def __repr__(self):
        return f"at {self.timestamp}ms -> {self.action}"


class Coder:
    id:                 int
    spawn_order:        int
    is_dead:            bool
    first_action:       Action
    prev_to_last:       Action
    last_action:        Action
    actions:            list[Action]
    times_compiled:     int
    times_debugged:     int
    times_refactored:   int
    dongles_locked:     int

    def _check_death(self) -> bool:
        self.is_dead = "burned out" in self.last_action.action
        return self.is_dead


    def __init__(self, id: int, spawn_order: int, action: Action):
        self.id = id
        self.spawn_order = spawn_order
        self.is_dead = "burned out" in action.action
        self.first_action = action
        self.prev_to_last = action
        self.last_action = action
        self.actions = [action]
        self._count_actions()


    def _count_actions(self):
        self.times_compiled = 0
        self.times_debugged = 0
        self.times_refactored = 0
        self.dongles_locked = 0
        for action in self.actions:
            if "is compiling" in action.action:
                self.times_compiled += 1
            elif "is debugging" in action.action:
                self.times_debugged += 1
            elif "is refactoring" in action.action:
                self.times_refactored += 1
            elif "has taken a dongle" in action.action:
                self.dongles_locked += 1


    def __bool__(self):
        return self.is_dead


    def __iadd__(self, action: Action):
        self.actions.append(action)
        self.prev_to_last = self.last_action
        self.last_action = self.actions[-1]
        self._check_death()
        self._count_actions()
        return self


    def __index__(self, idx: int) -> Action:
        return actions[idx]


    def __repr__(self) -> str:
        return (
            f"{CYAN}Coder{RESET}             : {self.id}{RESET}\n"
            f"is_dead{RESET}           : {RED+'yes' if self.is_dead else GREEN+'no'}{RESET}\n"
            f"first_action{RESET}      : {self.first_action}\n"
            f"prev_to_last{RESET}      : {self.prev_to_last}\n"
            f"last_action{RESET}       : {self.last_action}\n"
            f"num_actions{RESET}       : {len(self.actions)}\n"
            f"times_compiled{RESET}    : {self.times_compiled}\n"
            f"times_debugged{RESET}    : {self.times_debugged}\n"
            f"times_refactored{RESET}  : {self.times_refactored}\n"
            f"dongles_locked{RESET}    : {self.dongles_locked}\n"
        )


    def __str__(self) -> str:
        return self.__repr__()


class Simulation(typing.NamedTuple):
    scheduler:                   str
    number_of_coders:            int
    time_to_burnout:             int
    time_to_compile:             int
    time_to_debug:               int
    time_to_refactor:            int
    number_of_compiles_required: int
    dongle_cooldown:             int

    def __repr__(self):
        r = f"{CYAN}Simulation parameters:{RESET}\n"

        if self.number_of_coders <= 0:
            color = RED if self.number_of_coders < 0 else YELLOW
        else:
            color = GREEN
        r += f"number_of_coders:            {color}{self.number_of_coders}{RESET}\n"

        if self.time_to_burnout <= 0:
            color = RED if self.time_to_burnout < 0 else YELLOW
        else:
            color = GREEN
        r += f"time_to_burnout:             {color}{self.time_to_burnout}{RESET}\n"

        if self.time_to_compile <= 0:
            color = RED if self.time_to_compile < 0 else YELLOW
        else:
            color = GREEN
        r += f"time_to_compile:             {color}{self.time_to_compile}{RESET}\n"

        if self.time_to_debug <= 0:
            color = RED if self.time_to_debug < 0 else YELLOW
        else:
            color = GREEN
        r += f"time_to_debug:               {color}{self.time_to_debug}{RESET}\n"

        if self.time_to_refactor <= 0:
            color = RED if self.time_to_refactor < 0 else YELLOW
        else:
            color = GREEN
        r += f"time_to_refactor:            {color}{self.time_to_refactor}{RESET}\n"

        if self.number_of_compiles_required <= 0:
            color = RED if self.number_of_compiles_required < 0 else YELLOW
        else:
            color = GREEN
        r += f"number_of_compiles_required: {color}{self.number_of_compiles_required}{RESET}\n"

        if self.dongle_cooldown <= 0:
            color = RED if self.dongle_cooldown < 0 else YELLOW
        else:
            color = GREEN
        r += f"dongle_cooldown:             {color}{self.dongle_cooldown}{RESET}\n"

        if self.scheduler.lower() in {"edf", "fifo"}:
            color = MAGENTA if self.scheduler.lower() == "edf" else CYAN
            r += f"scheduler:                   {color}{self.scheduler}{RESET}\n"
        else:
            r += f"scheduler:                   {RED}unknown \"{self.scheduler}\"{RESET}\n"
        return r


def colorize_action(action: str):
    if "is compiling" in action:
        action = f"{YELLOW}{action}{RESET}"
    elif "is debugging" in action:
        action = f"{CYAN}{action}{RESET}"
    elif "is refactoring" in action:
        action = f"{CYAN}{action}{RESET}"
    elif "has taken a dongle" in action:
        action = f"{GREEN}{action}{RESET}"
    else:
        action = f"{RED}{action}{RESET}"
    return action


def receive_piped(file) -> list[Coder]:
    spawn_order = 1
    coders = {}
    for line in file.readlines():
        line = line.strip().split(maxsplit=2)
        if len(line) == 0:
            continue
        elif len(line) != 3:
            raise ValueError(f"Unknown action format: {line}")
        key = int(line[1])
        action = Action(int(line[0]), colorize_action(line[2]))
        if key in coders:
            coders[key] += action
        else:
            coders[key] = Coder(key, spawn_order, action)
            spawn_order += 1

    coder_list = sorted(list(coders.values()), key=lambda x: x.id)
    return coder_list


def get_dead_coder(coders: list[Coder]) -> Coder | None:
    for coder in coders:
        if coder.is_dead:
            return coder
    return None


def detect_simulation_stop(coders: list[Coder], argv: list[str]):
    dead_coder = get_dead_coder(coders) # Might be more than one in some error cases
    if len(argv) != 9:
        if dead_coder is not None:
            return f"{RED}coder {dead_coder.id} burned out!{RESET}"
        else:
            return (f"{YELLOW}no arguments passed, guessing:{RESET} "
                    f"{GREEN}compilation limit reached!{RESET}")
    else:
        simulation_params = Simulation(argv[8], *[int(x) for x in argv[1:8]])
        print(simulation_params)
        # TODO: implement math for checking simulation stop reasons
        if dead_coder is None:
            return f"{GREEN}compilation limit reached!{RESET}"
        return f"{RED}unknown stop reason{RESET}"


def get_end_reason(coders: list[Coder], argv: list[str]) -> str:
    return detect_simulation_stop(coders, argv)


def print_coders(coders: list[Coder]) -> str:
    for coder in coders:
        print(coder)


def exec_codexion(argv: list[str]) -> io.StringIO:
    try:
        proc = subprocess.run(argv, stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
    except FileNotFoundError as e:
        print(f"Aborting due to Exception: {e}", file=sys.stderr)
        exit(128)
    pseudo_file = io.StringIO(proc.stdout.decode())
    if proc.returncode != 0:
        print(f"Aborting due to Codexion error: {pseudo_file.read()}", file=sys.stderr)
        exit(129)
    return pseudo_file


def main(argv: list[str]) -> int:
    if not sys.stdin.isatty():
        coder_list: list[Coder] = receive_piped(sys.stdin)
    else:
        argv = argv[1:]
        pseudo_file = exec_codexion(argv)
        coder_list: list[Coder] = receive_piped(pseudo_file)
    end_reason = get_end_reason(coder_list, argv)
    print_coders(coder_list)
    print(f"Simulation ended due to: {end_reason}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))

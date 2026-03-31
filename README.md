*This project has been created as part of the 42 curriculum by lumarcuc*

# Codexion

##  Description

> This project is an introduction on [multithreading](https://en.wikipedia.org/wiki/Multithreading_(computer_architecture)) in C, and how to handle concurrent ressource management using the [POSIX_THREADS](https://man7.org/linux/man-pages/man7/pthreads.7.html) library.
>
> In short, we are asked to create *coders*, threads that share a ressource with their neighbour called a *dongle* that is used in their *routine*.
> 
> A coder:
- compiles (using his two dongles)
- debugs
- refactors
>
> All these parameters are specified (in ms for the cooldowns) in the input. Two neighbouring coders CANNOT compile at the same time, since they share one dongle and, while it is used, its only available to the one compiling. 
> 
> Those dongles also posses their own *cooldown*, during which they are not available to any coder.
> 
> It is also asked of us to manage neighbouring coders trying to access the same dongle at the same time using a queue on the heap following these two principles:
- FIFO (first in, first out)
- EDF (earliest deadline first)
>
> The programm ends if;
- one coder has burned out
- all coders have compiled enough to reach the goal
>
> Burning out and compilation goal are both parameters specified in the input. Burning out meaning that the time specified in *burnout time* has passed since the last compilation of that coder.
>
> This is regularily checked by a *monitor* thread, that supervises the coders and checks burnout times.
>
> Thread management is specified [below](#thread-sync).
##  Instructions

### Building codexion

A regular Makefile is at your disposal.

The flags for compilation are: *-Wall -Werror -Wextra -pthread*

`make` will build the program  named **codexion**.

`make all` does the same.

`make clean` clears out the .o files.

`make fclean` clears out the program and the .o files.

`make re` cleans and re-compilates everything.

### Running codexion
`./codexion 'arguments'` to run the simulation.

Arguments are detailed like so:

1. Number of coders (in decimal format).
2. Time to burnout (in ms).
3. Time to compile (in ms).
4. Time to debug (in ms).
5. Time to refactor (in ms).
6. Compilation goal (in decimal format).
7. Dongle cooldown (in ms).
8. Scheduler manager (one of `fifo` or `edf`).

If anything is misspelled / the int values are negative or higher than INT_MAX, you will get an error.

##  Resources
[basic threading principles](https://www.geeksforgeeks.org/c/thread-functions-in-c-c/)

[helgrind for threading](https://valgrind.org/docs/manual/hg-manual.html)

[timed_wait manual](https://www.man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html)
### AI Usage
> I lightly used mistral ai for some valgrind debugging / getting around on how to actually debug threads.

##  Blocking cases handled
> **Deadlock** is what you want to avoid when multithreading.
>
> To avoid deadlocks, no thread is ever left on a lock try that is a condition. The only case where this may have been an issue is for 1 coder. I have 'hardcoded' the solution in which the coder (alone) just waits for the monitor thread to tell him that he is starved.
> 
> A robust queue system in a `while(dongles are not free || you do not have priority)` into `pthread_cond_waits` guards any thread of jumping ahead of others.
>
> Under `edf`, threads will evaluate their priority based on **last time compiled**, and so starvation will be avoided at all cost.
>
Coffman's conditions are avoided as follows:
- Mutual exclusion: **queue** is guarded by `queue_mutex`, **dongles** by their own mutexes, **shared data** by a `work mutex` and **output** by a `output mutex`.
- Hold on and wait: no coder holds any critical information that will permalock another. Every thread is guaranteed (under doable rules) to get into compilation.
- No preemption: no thread can be forced out of a process by another one grabbing its ressources. The only time a coder gets any information from outside are from `broadcasts` or the `active` boolean from the monitor thread.
- Circular wait: the waiting pattern for accessing the **code routine** is handled by a queue **heap**, that gets re-checked each time a coder is done with other ressources. This means no indefinite wait.

##  Thread synchronization mechanisms

> To safeguard the ressources shared, I used a number of mutexes for every task.
> 
> The queue is blocked by a mutex named `queue_mutex`. If you do not meet the requirements to acquire dongles, you are stuck between `pthread_cond_timedwait` (there was a dongle on cooldown, and you wake up when its over to check again) and `pthread_cond_wait` (your neighbour is actively using dongles OR the queue **scheduler** decided you do not have priority in the queue). Every time a coder finishes compiling and has freed the dongles and set their avaibility back to `TRUE`, the thread will call `pthread_cond_broadcast` on the queue mutex, telling all waiting coders to check again on if they can compile and get out of queue.
  
> Each dongle has a mutex. This in theory is a safeguard that is never crossed since the queue mutex protects the dongle access. Before compiling, a coder thread locks both of the neighbouring dongle mutexes, and unlocks them when he is done.
>
> Each coder protects its compile numbers by a `compile_mutex` and the `active` boolean state for the simulation is also protected by an `active_mutex`, shared by the monitoring thread and the coders.
>
> For logging, the same principle is at play with an `output mutex`. Before printing any state message of any kind, each thread (including the **monitor**) has to acquire the output mutex and then release it. This way, every message is serialized.
>
> To inscribe last compile time for `edf` management, each coder has a `burnout_mutex`, that he will lock and unlock when inscribing that data.
>
> For extra precision on burnouts / compile goal reached, we have a boolean in `active` that is checked just before printing logs, to make sure that we are not done and that the monitor thread has not signified the end of the programm.
> 
> The monitor thread is also subject to these mutex locks / unlocks to guarantee that data is always the way it should be when looked at / worked on.


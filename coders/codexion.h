/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:07:07 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/04 14:10:52 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <sys/time.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

typedef enum e_BOOl
{
	FALSE,
	TRUE
}	t_BOOL;

typedef enum e_log
{
	ARGS,
	INT,
	SCHEDULE
}	t_log;

typedef enum e_schedule
{
	EDF = 1,
	FIFO = 2
}	t_schedule;

typedef enum e_end
{
	BURNOUT,
	SUCCESS
}	t_end;

typedef struct s_data
{
	unsigned int	coders;
	unsigned int	burnout_time;
	unsigned int	compile_time;
	unsigned int	debug_time;
	unsigned int	refactor_time;
	unsigned int	compile_goal;
	unsigned int	dongle_time;
	t_schedule		scheduler;
}	t_data;

typedef struct s_queue
{
	void			*coder;
	unsigned int	coder_id;
	struct timeval	last_compile;
	unsigned int	burnout_time;
	struct s_queue	*next;
}	t_queue;

typedef enum e_queue_arg
{
	ADD,
	REMOVE
}	t_queue_arg;

typedef enum e_type
{
	COMPILING,
	REFACTORING,
	DEBUGGING,
	TAKE
}	t_type;

typedef struct s_dongle
{
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*queue_mutex;
	pthread_cond_t	*wait;
	t_BOOL			free;
	struct timeval	next_free;
	t_queue			*waiting_list;
}	t_dongle;

typedef struct s_coder
{
	t_data			data;
	unsigned int	nb_compiles;
	pthread_mutex_t	*compile_mutex;
	pthread_mutex_t	*output_mutex;
	pthread_mutex_t	*active_mutex;
	pthread_mutex_t	*burnout_mutex;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	unsigned int	id;
	struct timeval	start_time;
	struct timeval	*last_compile;
	struct timespec	cooldown;
	t_BOOL			*active;
	t_BOOL			finished;
}	t_coder;

typedef struct s_control
{
	t_data			data;
	pthread_t		thread;
	pthread_mutex_t	*active_mutex;
	pthread_mutex_t	*output_mutex;
	struct timeval	start_time;
	t_coder			*coders;
	t_dongle		*dongles;
	t_BOOL			*active;
}	t_control;

typedef void	(*t_queue_op)(t_coder *coder, t_dongle *dongle);
// parsing
t_data			convert_input_to_data(int ac, char **av);
t_BOOL			ft_validate_input(int ac, char **av);
int				check_str(char *str);
t_BOOL			check_int(char *str);
t_BOOL			log_error(int *nb_error, t_log type, int arg_index, char *arg);
// startup
void			generate_dongles(t_control *controller);
void			generate_coders(t_control *controller);
t_control		init_controller(t_data data);
void			assign_more(t_coder *coders, t_control *controller,
					unsigned int id);
void			start_threads(t_control controller);
// threads
void			*surveil(void *arg);
void			*code(void *arg);
void			*rot_in_hell(t_coder *coder);
// work routine
void			work(t_coder *coder);
void			work_schedule(t_coder *coder, t_type operation);
void			log_status(t_coder *coder, t_type operation);
void			wait_cooldown(t_type operation, t_data data);
// dongle manipulation
void			ask_for_dongles(t_coder *coder);
void			lock_dongle(t_coder *coder, t_dongle *dongle);
void			unlock_dongles(t_coder *coder);
struct timespec	*convert_dongle(struct timeval time, struct timespec *new);
void			inscribe_dongle_data(t_coder *coder);
// timing
long long		get_total_time_timeval(struct timeval time);
t_BOOL			is_burned_out(struct timeval time,
					struct timeval old, int burnout_time);
long long		get_actual_time_ms(struct timeval start, struct timeval now);
// free
void			free_everything(t_control *controller);
void			liberate_threads(t_control *controller);
void			free_queue(t_queue *queue);
// supervisor / end conditions
void			*end(struct timeval time, t_end signal,
					int id, t_control *controller);
t_BOOL			everyone_is_finished(t_control *controller);
t_BOOL			is_finished(t_coder *coder);
t_BOOL			supervisor_said_its_over(t_coder *coder);
// queue
void			queue_manipulation(t_coder *coder, t_queue_arg signal,
					t_dongle *dongle);
void			add_to_waiting_list(t_coder *coder, t_dongle *dongle);
void			remove_from_waiting_list(t_coder *coder, t_dongle *dongle);
void			signal_neighbours(t_coder *coder);
// priority
t_BOOL			i_have_priority(t_coder *coder, t_dongle *dongle);
t_BOOL			priority(t_coder *coder, t_dongle *dongle);
t_BOOL			neighbour_not_elligible(t_coder *coder, t_dongle *dongle);
t_BOOL			im_first_in(t_coder *coder, t_dongle *dongle);
t_BOOL			i_will_burn_out(t_coder *coder, t_dongle *dongle);

#endif

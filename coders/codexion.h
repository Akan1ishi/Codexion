/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:07:07 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/18 15:47:55 by lumarcuc         ###   ########.fr       */
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
} t_BOOL;

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
	t_schedule	scheduler;
}	t_data;

typedef struct s_coder_info
{
	int		compile_time;
	int		debug_time;
	int		refactor_time;
	int		dongle_cooldown;
	int		total_coders;
	t_schedule	scheduler;
	int		burnout_time;
}	t_coder_info;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	t_BOOL		free;
	struct timeval	next_free;
}	t_dongle;

typedef enum e_type
{
	COMPILING,
	REFACTORING,
	DEBUGGING,
	TAKE
}	t_type;

typedef enum e_signal
{
	LOCK,
	UNLOCK
}	t_signal;

typedef struct s_queue
{
	void		*coder;
	struct s_queue	*next;
}	t_queue;

typedef struct s_coder
{
	t_coder_info	data;
	unsigned int	*nb_compiles;
	pthread_mutex_t	*global_mutex;
	pthread_mutex_t	*output_mutex;
	pthread_cond_t	*global_cond;
	pthread_mutex_t	*work_mutex;
	pthread_t	thread;
	t_dongle	*left;
	t_dongle	*right;
	int		id;
	struct timeval	start_time;
	struct timeval	*last_compile;
	struct timespec	cooldown;
	t_queue		**queue;
	t_BOOL		*active;
}	t_coder;

typedef struct s_control
{
	t_data		data;
	pthread_t	thread;
	pthread_mutex_t	global_mutex;
	pthread_mutex_t	output_mutex;
	pthread_mutex_t	work_mutex;
	pthread_cond_t	global_cond;
	struct timeval	start_time;
	t_coder		*coders;
	t_dongle	*dongles;
	unsigned int	*nb_compiles;
	t_queue		*queue;
	t_BOOL		*active;
}	t_control;


typedef int (*mutex_op)(pthread_mutex_t *);
// parsing
t_data		convert_input_to_data(int ac, char **av);
t_BOOL		ft_validate_input(int ac, char **av);
int		check_str(char *str);
t_BOOL		check_int(char *str);
void		log_error(int nb_error, t_log type, int arg_index);
// startup
void		generate_dongles(t_control *controller);
void		generate_coders(t_control *controller);
t_control	init_controller(t_data data);
void		start_threads(t_control controller);
// threads
void		*surveil(void *arg);
void		*code(void *arg);
void		*rot_in_hell(t_coder *coder);
// coder inside funcs
long long	get_actual_time_ms(struct timeval beginning, struct timeval current);
void		log_status(t_coder *coder, t_type operation);
void		wait_cooldown(t_type operation, t_coder_info data);
void		work_schedule(t_coder *coder, t_type operation);
// dongle functions
mutex_op	get_mutex_op(t_signal signal);
void		manage_dongles(t_coder *coder, t_signal signal);
void		inscribe_dongle_data(t_coder *coder);
t_BOOL		dongles_are_free(t_coder *coder);
t_BOOL		dongles_on_cooldown(t_coder *coder);
struct timespec	*convert_longest_dongle(t_coder *coder, struct timespec *cooldown);
long long	get_total_time_timeval(struct timeval time);
// queue
void		add_to_queue(t_queue **queue, t_coder *coder);
t_BOOL		next_in_queue(t_coder *coder, t_queue  *queue);
void		remove_from_queue(t_queue **queue, t_coder *coder);
t_BOOL		in_queue(t_queue *queue, int id);
t_BOOL		has_priority(int id, int neighbour_id, t_schedule scheduler, t_queue *queue);
t_BOOL		appears_before(int id, int neighbour_id, t_queue *queue);
//
void		signal_end(struct timeval time, t_end signal, int id, t_control *controller);
t_BOOL		is_burned_out(struct timeval time, struct timeval old, int burnout_time);
// free
void		free_everything(t_control *controller);
void		liberate_threads(t_control *controller);
void		free_queue(t_queue *queue);
#endif

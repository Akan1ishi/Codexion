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

typedef enum e_schedule
{
	EDF = 1,
	FIFO = 2
}	t_schedule;

typedef struct s_data
{
	int		coders;
	int		burnout_time;
	int		compile_time;
	int		debug_time;
	int		refactor_time;
	int		compile_goal;
	int		dongle_time;
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
	struct timeval	time_in_queue;
	struct s_queue	*next;
}	t_queue;

typedef struct s_coder
{
	t_coder_info	data;
	unsigned int	*nb_compiles;
	pthread_mutex_t	*global_mutex;
	pthread_mutex_t	*output_mutex;
	pthread_cond_t	*global_cond;
	pthread_t	thread;
	t_dongle	*left;
	t_dongle	*right;
	int		id;
	struct timeval	start_time;
	struct timeval	last_compile;
	struct timespec	cooldown;
	t_queue		*queue;
}	t_coder;

typedef struct s_control
{
	t_data		data;
	pthread_t	thread;
	pthread_mutex_t	global_mutex;
	pthread_cond_t	global_cond;
	pthread_mutex_t	output_mutex;
	struct timeval	start_time;
	t_coder		*coders;
	t_dongle	*dongles;
	unsigned int	*nb_compiles;
	t_queue		*queue;
}	t_control;


typedef int (*mutex_op)(pthread_mutex_t *);
// startup
void		generate_dongles(t_control *controller);
void		generate_coders(t_control *controller);
t_control	init_controller(t_data data);
void		start_threads(t_control controller);
// threads
void		*surveil(void *arg);
void		*code(void *arg);
void		*test_surveil(void *arg);
void		*test_code(void *arg);
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
#endif

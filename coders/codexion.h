/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:07:07 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/17 19:04:42 by lumarcuc         ###   ########.fr       */
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

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	active;
	pthread_t		id;
	long long		cooldown;
	void	(*run)(const struct s_dongle*);
	void	(*resolve)(t_schedule);
}	t_dongle;

typedef enum e_type
{
	COMPILING,
	REFACTORING,
	DEBUGGING,
	NB_OPS
}	t_type;

typedef struct s_coder
{
	pthread_t	id;
	t_dongle	*left;
	t_dongle	*right;
	long long	start_time;
	long long	last_compile;
	t_BOOL		starved;
	void	(*run)(const struct s_coder*);
	void	(*print)(long long, t_type);
}	t_coder;

typedef struct s_control
{
	pthread_t	id;
	t_data		data;
	t_coder		**coders;
	t_dongle	**dongles;
	t_schedule	scheduler;


}	t_control;

t_BOOL	ft_validate_input(int ac, char **av);
int	check_str(char *str);
t_BOOL	check_int(char *str);
t_data	convert_input_to_data(int ac, char **av);
#endif

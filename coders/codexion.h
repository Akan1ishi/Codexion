/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:07:07 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/12 18:11:04 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdbool.h>
# include <sys/time.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>

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

} t_data;

bool	ft_validate_input(int ac, char **av);
int	check_str(char *str);
bool	check_int(char *str);
t_data	convert_input_to_data(int ac, char **av);
#endif

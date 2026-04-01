/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threader.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 14:04:19 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/01 18:23:46 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	*surveil(void *arg)
{
	t_control		*controller;
	struct timeval	current_time;
	unsigned int	i;

	controller = (t_control *) arg;
	while (TRUE)
	{
		gettimeofday(&current_time, NULL);
		i = 0;
		while (i < controller->data.coders)
		{
			pthread_mutex_lock(controller->coders[i].burnout_mutex);
			if (is_burned_out(current_time, *controller->coders[i].last_compile,
					controller->data.burnout_time) == TRUE)
				return (end(current_time, BURNOUT, i, controller));
			pthread_mutex_unlock(controller->coders[i].burnout_mutex);
			i++;
		}
		if (everyone_is_finished(controller) == TRUE)
			return (end(current_time, SUCCESS, 1, controller));
		usleep(100);
	}
	return (NULL);
}

void	*code(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	while (TRUE)
	{
		if (supervisor_said_its_over(coder) == TRUE)
			break ;
		queue_manipulation(coder, ADD);
		wait_on_dongles(coder);
		queue_manipulation(coder, REMOVE);
		work(coder);
		if (is_finished(coder) == TRUE)
			break ;
	}
	return (NULL);
}

void	*rot_in_hell(t_coder *coder)
{
	while (TRUE)
	{
		usleep(1000);
		if (supervisor_said_its_over(coder) == TRUE)
			break ;
	}
	return (NULL);
}

void	work(t_coder *coder)
{
	work_schedule(coder, COMPILING);
	if (coder->nb_compiles == coder->data.compile_goal)
	{
		pthread_mutex_lock(coder->compile_mutex);
		coder->finished = TRUE;
		pthread_mutex_unlock(coder->compile_mutex);
		return ;
	}
	work_schedule(coder, DEBUGGING);
	work_schedule(coder, REFACTORING);
}

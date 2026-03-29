/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threader.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 14:04:19 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/17 17:49:56 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	*surveil(void *arg)
{
	t_control	*controller;
	struct timeval	current_time;
	unsigned int	index;

	controller = (t_control *) arg;
	while (TRUE)
	{
		gettimeofday(&current_time, NULL);
		if (*controller->nb_compiles >= controller->data.compile_goal)
		{
			signal_end(current_time, SUCCESS, 0, controller);
			break;
		}
		index = 0;
		while (index < controller->data.coders)
		{
			if (is_burned_out(current_time, *controller->coders[index].last_compile, controller->data.burnout_time) == TRUE)
			{
				signal_end(current_time, BURNOUT, index + 1, controller);
				break;
			}
			index++;
		}
		if (*controller->active == FALSE)
			break;

	}
	return NULL;
}

void	*code(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	if (coder->data.total_coders == 1)
		return (rot_in_hell(coder));
	while (TRUE)
	{
		if (*coder->active == FALSE)
			break;
		pthread_mutex_lock(coder->global_mutex);
		add_to_queue(coder->queue, coder);
		while (dongles_are_free(coder) == FALSE || next_in_queue(coder, *coder->queue) == FALSE)
		{
			if (dongles_on_cooldown(coder) == TRUE)
				pthread_cond_timedwait(coder->global_cond, coder->global_mutex, convert_longest_dongle(coder, &coder->cooldown));
			else
				pthread_cond_wait(coder->global_cond, coder->global_mutex);
		}
		remove_from_queue(coder->queue, coder);
		work_schedule(coder, COMPILING);
		work_schedule(coder, DEBUGGING);
		work_schedule(coder, REFACTORING);
	}
	return NULL;
}

void	*rot_in_hell(t_coder *coder)
{
	while (TRUE)
	{
		usleep(1000);
		if (*coder->active == FALSE)
			break;
	}
	return (NULL);
}

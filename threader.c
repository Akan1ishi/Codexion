/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threader.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 14:04:19 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/29 17:22:35 by lumarcuc         ###   ########.fr       */
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
		pthread_mutex_lock(&controller->work_mutex);
		if (*controller->nb_compiles >= controller->data.compile_goal)
			return (end(current_time, SUCCESS, 0, controller));
		pthread_mutex_unlock(&controller->work_mutex);
		i = 0;
		while (i < controller->data.coders)
		{
			pthread_mutex_lock(&controller->coders[i].burnout_mutex);
			if (is_burned_out(current_time, *controller->coders[i].last_compile,
					controller->data.burnout_time) == TRUE)
				return (end(current_time, BURNOUT, 0, controller));
			pthread_mutex_unlock(&controller->coders[i].burnout_mutex);
			i++;
		}
	}
	return (NULL);
}

void	*code(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	if (coder->data.coders == 1)
		return (rot_in_hell(coder));
	while (TRUE)
	{
		if (*coder->active == FALSE)
			break ;
		pthread_mutex_lock(coder->queue_mutex);
		add_to_queue(coder->queue, coder);
		while (dongles_are_free(coder) == FALSE
			|| next_in_queue(coder, *coder->queue) == FALSE)
		{
			if (dongles_on_cooldown(coder) == TRUE)
				pthread_cond_timedwait(coder->queue_cond, coder->queue_mutex,
					convert_longest_dongle(coder, &coder->cooldown));
			else
				pthread_cond_wait(coder->queue_cond, coder->queue_mutex);
		}
		remove_from_queue(coder->queue, coder);
		pthread_mutex_unlock(coder->queue_mutex);
		work(coder);
	}
	return (NULL);
}

void	*rot_in_hell(t_coder *coder)
{
	while (TRUE)
	{
		usleep(1000);
		if (*coder->active == FALSE)
			break ;
	}
	return (NULL);
}

t_BOOL	appears_before(int id, int neighbour_id, t_queue *queue)
{
	while (queue)
	{
		if (((t_coder *) queue->coder)->id == (unsigned int) id)
			return (TRUE);
		if (((t_coder *) queue->coder)->id == (unsigned int) neighbour_id)
			return (FALSE);
		queue = queue->next;
	}
	return (FALSE);
}

void	work(t_coder *coder)
{
	work_schedule(coder, COMPILING);
	work_schedule(coder, DEBUGGING);
	work_schedule(coder, REFACTORING);
}

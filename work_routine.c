/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 14:30:01 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/04 13:18:44 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	work(t_coder *coder)
{
	work_schedule(coder, COMPILING);
	if (coder->nb_compiles == coder->data.compile_goal)
	{
		pthread_mutex_lock(coder->compile_mutex);
		coder->finished = TRUE;
		pthread_mutex_unlock(coder->compile_mutex);
		unlock_dongles(coder);
		return ;
	}
	unlock_dongles(coder);
	work_schedule(coder, DEBUGGING);
	work_schedule(coder, REFACTORING);
}

void	work_schedule(t_coder *coder, t_type operation)
{
	if (operation != COMPILING)
	{
		log_status(coder, operation);
		wait_cooldown(operation, coder->data);
		return ;
	}
	log_status(coder, operation);
	wait_cooldown(operation, coder->data);
	coder->nb_compiles += 1;
	inscribe_dongle_data(coder);
}

void	log_status(t_coder *coder, t_type operation)
{
	struct timeval	tz;
	long long		actual_time;

	gettimeofday(&tz, NULL);
	actual_time = get_actual_time_ms(coder->start_time, tz);
	pthread_mutex_lock(coder->output_mutex);
	if (supervisor_said_its_over(coder) == FALSE)
	{
		if (operation == COMPILING)
			printf("%lld %d is compiling\n", actual_time, coder->id);
		else if (operation == REFACTORING)
			printf("%lld %d is refactoring\n", actual_time, coder->id);
		else if (operation == DEBUGGING)
			printf("%lld %d is debugging\n", actual_time, coder->id);
		else if (operation == TAKE)
			printf("%lld %d has taken a dongle\n", actual_time, coder->id);
	}
	pthread_mutex_unlock(coder->output_mutex);
}

void	wait_cooldown(t_type operation, t_data data)
{
	if (operation == COMPILING)
		usleep(data.compile_time * 1000);
	else if (operation == REFACTORING)
		usleep(data.refactor_time * 1000);
	else
		usleep(data.debug_time * 1000);
}

void	inscribe_dongle_data(t_coder *coder)
{
	struct timeval	tz;

	gettimeofday(&tz, NULL);
	pthread_mutex_lock(coder->burnout_mutex);
	*coder->last_compile = tz;
	pthread_mutex_unlock(coder->burnout_mutex);
	tz.tv_sec += coder->data.dongle_time / 1000;
	tz.tv_usec += (coder->data.dongle_time * 1000) % 1000000;
	coder->right->next_free = tz;
	coder->left->next_free = tz;
	coder->right->free = TRUE;
	coder->left->free = TRUE;
}

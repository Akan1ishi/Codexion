/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 14:30:01 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/03 18:37:38 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

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

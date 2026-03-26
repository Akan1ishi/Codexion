/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                   +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:07:37 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/18 11:39:20 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	*test_surveil(void *arg)
{
	return NULL;
}

void	*test_code(void *arg)
{
	t_coder	*coder;

	int i = 0;
	coder = (t_coder *) arg;
	while (TRUE)
	{
		pthread_mutex_lock(coder->global_mutex);
		add_to_queue(&coder->queue, coder);
		while (dongles_are_free(coder) == FALSE || next_in_queue(coder, coder->queue) == FALSE)
		{
			//printf("[DEBUG] %d in waiting room\n", coder->id);
			if (dongles_on_cooldown(coder) == TRUE)
			{
				//printf("[DEBUG] %d in cond_timedwait\n", coder->id);
				pthread_cond_timedwait(coder->global_cond, coder->global_mutex, convert_longest_dongle(coder, &coder->cooldown));
			}
			else
			{
				//printf("[DEBUG] %d in cond_wait\n", coder->id);
				pthread_cond_wait(coder->global_cond, coder->global_mutex);
			}
		}
		remove_from_queue(&coder->queue, coder);
		work_schedule(coder, COMPILING);
		work_schedule(coder, DEBUGGING);
		work_schedule(coder, REFACTORING);
		if (i == 20)
			break;
		i++;
	}
	return NULL;
}
int	main(int ac, char **av)
{
	t_data	data;
	t_control	controller;
	data.coders = 4;
	data.burnout_time = 5000;
	data.compile_time = 50;
	data.debug_time = 25;
	data.refactor_time = 34;
	data.compile_goal = 100;
	data.dongle_time = 300;
	data.scheduler = FIFO;

	controller = init_controller(data);
	start_threads(controller);
	printf("%d\n", *controller.nb_compiles);
	return (0);
}

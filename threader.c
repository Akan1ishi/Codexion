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
	t_control	controller;

	controller = *(t_control *) arg;
	printf("CONTROLLER ONLINE\n");
	return (NULL);
}

void	*code(void *arg)
{
	t_coder	coder;

	coder = *(t_coder *) arg;
	pthread_mutex_lock(coder.global_mutex);
	add_to_queue(coder);
	while (TRUE)
	{
		while (dongles_free(coder) == FALSE || next_in_queue(coder) == FALSE)
		{
			if (dongles_under_cooldown(coder) == TRUE)
				pthread_cond_timedwait(&coder.global_cond, &coder.global_mutex, get_dongle_timing(coder));
			else
				pthread_cond_wait(&coder.global_cond, &coder.global_mutex);
		}
		lock_dongles(coder);
		remove_from_queue(coder);
		pthread_mutex_unlock(&coder.global_mutex);
		compile(coder);
		unlock_dongles(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}

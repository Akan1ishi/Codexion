/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 14:29:19 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/03 18:35:35 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	free_everything(t_control *controller)
{
	unsigned int	i;

	liberate_threads(controller);
	i = 0;
	while (i < controller->data.coders)
	{
		free(controller->coders[i].last_compile);
		free_queue(controller->dongles[i].waiting_list);
		i++;
	}
	free(controller->coders);
	free(controller->dongles);
}

void	liberate_threads(t_control *controller)
{
	unsigned int	i;

	i = 0;
	while (i < controller->data.coders)
	{
		pthread_mutex_destroy(controller->dongles[i].mutex);
		free(controller->dongles[i].mutex);
		pthread_mutex_destroy(controller->dongles[i].queue_mutex);
		free(controller->dongles[i].queue_mutex);
		pthread_cond_destroy(controller->dongles[i].wait);
		free(controller->dongles[i].wait);
		pthread_mutex_destroy(controller->coders[i].burnout_mutex);
		pthread_mutex_destroy(controller->coders[i].compile_mutex);
		free(controller->coders[i].burnout_mutex);
		free(controller->coders[i].compile_mutex);
		i++;
	}
	pthread_mutex_destroy(controller->output_mutex);
	pthread_mutex_destroy(controller->active_mutex);
	free(controller->active);
	free(controller->active_mutex);
	free(controller->output_mutex);
}

void	free_queue(t_queue *queue)
{
	t_queue	*temp;

	while (queue)
	{
		temp = queue->next;
		free(queue);
		queue = temp;
	}
}

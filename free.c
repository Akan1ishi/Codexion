/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 14:29:19 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/29 17:27:42 by lumarcuc         ###   ########.fr       */
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
		i++;
	}
	free(controller->coders);
	free(controller->dongles);
	free(controller->active);
	free(controller->nb_compiles);
	free_queue(controller->queue);
}

void	liberate_threads(t_control *controller)
{
	unsigned int	i;

	i = 0;
	while (i < controller->data.coders)
	{
		pthread_mutex_destroy(&controller->dongles[i].mutex);
		pthread_mutex_destroy(&controller->coders[i].burnout_mutex);
		i++;
	}
	pthread_mutex_destroy(&controller->queue_mutex);
	pthread_mutex_destroy(&controller->output_mutex);
	pthread_mutex_destroy(&controller->work_mutex);
	pthread_cond_destroy(&controller->queue_cond);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 14:28:49 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/01 11:59:57 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	add_to_queue(t_queue **queue, t_coder *coder)
{
	t_queue	*element;
	t_queue	*temp;

	pthread_mutex_lock(coder->queue_mutex);
	element = malloc(sizeof(t_queue) * 1);
	element->coder = coder;
	element->next = NULL;
	if (*queue == NULL)
		*queue = element;
	else
	{
		temp = *queue;
		while (temp->next)
			temp = temp->next;
		temp->next = element;
	}
	pthread_mutex_unlock(coder->queue_mutex);
}

t_BOOL	next_in_queue(t_coder *coder, t_queue *queue)
{
	int	left;
	int	right;
	t_BOOL	result;

	result = TRUE;
	if (coder->id == 1)
		left = coder->data.coders;
	else
		left = coder->id - 1;
	if (coder->id == coder->data.coders)
		right = 1;
	else
		right = coder->id + 1;
	pthread_mutex_lock(coder->queue_mutex);
	if (in_queue(queue, right) == TRUE)
		if (priority(coder->id, right, coder->data.scheduler, queue) == FALSE)
			return (FALSE);
	if (result == TRUE && in_queue(queue, left) == TRUE)
		if (priority(coder->id, left, coder->data.scheduler, queue) == FALSE)
			result = FALSE;
	pthread_mutex_unlock(coder->queue_mutex);
	return (result);
}

void	remove_from_queue(t_queue **queue, t_coder *coder)
{
	t_queue	*temp;
	t_queue	*element;

	pthread_mutex_lock(coder->queue_mutex);
	temp = *queue;
	if (*queue && ((t_coder *)temp->coder)->id == coder->id)
	{
		element = *queue;
		*queue = (*queue)->next;
		free(element);
		pthread_mutex_unlock(coder->queue_mutex);
		return ;
	}
	while (temp->next && ((t_coder *)temp->next->coder)->id != coder->id)
		temp = temp->next;
	element = temp->next;
	temp->next = element->next;
	free(element);
	pthread_mutex_unlock(coder->queue_mutex);
}

t_BOOL	in_queue(t_queue *queue, unsigned int id)
{
	while (queue)
	{
		if (((t_coder *)queue->coder)->id == id)
			return (TRUE);
		queue = queue->next;
	}
	return (FALSE);
}

t_BOOL	priority(int id, int nid, t_schedule scheduler, t_queue *queue)
{
	t_queue	*neighbour;
	t_queue	*coder;
	t_coder	*self;
	t_coder	*neighbour_coder;

	if (scheduler == FIFO)
	{
		if (appears_before(id, nid, queue) == TRUE)
			return (TRUE);
	}
	neighbour = queue;
	coder = queue;
	while (((t_coder *)neighbour->coder)->id != (unsigned int) id)
		neighbour = neighbour->next;
	neighbour_coder = (t_coder *)neighbour->coder;
	while (((t_coder *)coder->coder)->id != (unsigned int) id)
		coder = coder->next;
	self = (t_coder *)coder->coder;
	if (get_total_time_timeval(*self->last_compile) + self->data.burnout_time
		<= get_total_time_timeval(*neighbour_coder->last_compile)
		+ neighbour_coder->data.burnout_time)
		return (TRUE);
	return (FALSE);
}

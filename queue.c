#include "coders/codexion.h"

void	add_to_queue(t_queue **queue, t_coder *coder)
{
	t_queue	*element;
	t_queue	*temp;

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
}

t_BOOL	next_in_queue(t_coder *coder, t_queue *queue)
{
	int	left;
	int 	right;

	if (coder->id == 1)
		left = coder->data.total_coders;
	else
		left = coder->id - 1;
	if (coder->id == coder->data.total_coders)
		right = 1;
	else
		right = coder->id + 1;
	if (in_queue(queue, right) == TRUE)
	{
		if (has_priority(coder->id, right, coder->data.scheduler, queue) == FALSE)
			return (FALSE);
	}
	if (in_queue(queue, left) == TRUE)
	{
		if (has_priority(coder->id, left, coder->data.scheduler, queue) == FALSE)
			return (FALSE);
	}
	return (TRUE);
}


void	remove_from_queue(t_queue **queue, t_coder *coder)
{
	t_queue	*temp;
	t_queue	*element;

	temp = *queue;
	if (*queue && ((t_coder *)temp->coder)->id == coder->id)
	{
		element = *queue;
		*queue = (*queue)->next;
		free(element);
		return ;
	}
	while (temp->next && ((t_coder *)temp->next->coder)->id != coder->id)
		temp = temp->next;
	element = temp->next;
	temp->next = element->next;
	free(element);
}

t_BOOL	in_queue(t_queue *queue, int id)
{
	while (queue)
	{
		if (((t_coder *)queue->coder)->id == id)
			return (TRUE);
		queue = queue->next;
	}
	return (FALSE);
}

t_BOOL	has_priority(int id, int neighbour_id, t_schedule scheduler, t_queue *queue)
{
	t_queue	*neighbour;
	t_queue	*coder;
	t_coder	*self;
	t_coder	*neighbour_coder;

	if (scheduler == FIFO)
	{
		if (appears_before(id, neighbour_id, queue) == TRUE)
			return (TRUE);
	}
	neighbour = queue;
	coder = queue;
	while (((t_coder *)neighbour->coder)->id != neighbour_id)
		neighbour = neighbour->next;
	neighbour_coder = (t_coder *)neighbour->coder;
	while (((t_coder *)coder->coder)->id != id)
		coder = coder->next;
	self = (t_coder *)coder->coder;
	if (get_total_time_timeval(*self->last_compile) + self->data.burnout_time <= get_total_time_timeval(*neighbour_coder->last_compile) + neighbour_coder->data.burnout_time)
		return (TRUE);
	return (FALSE);
}

t_BOOL	appears_before(int id, int neighbour_id, t_queue *queue)
{
	while (queue)
	{
		if (((t_coder *) queue->coder)->id == id)
			return TRUE;
		if (((t_coder *) queue->coder)->id == neighbour_id)
			return FALSE;
		queue = queue->next;
	}
	return FALSE;
}

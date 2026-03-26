#include "coders/codexion.h"

void	add_to_queue(t_queue **queue, t_coder *coder)
{
	t_queue	*element;

	element = malloc(sizeof(t_queue) * 1);
	element->coder = coder;
	gettimeofday(&element->time_in_queue, NULL);
	element->next = NULL;
	if (*queue == NULL)
		*queue = element;
	else
		(*queue)->next = element;
	
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
	if (((t_coder *)temp->coder)->id == coder->id)
	{
		element = *queue;
		temp = temp->next;
		*queue = temp;
		free(element);
		return ;
	}
	while (((t_coder *)temp->next->coder)->id != coder->id)
		temp = temp->next;
	element = temp->next;
	temp->next = element->next;
	free(element);
	*queue = temp;
}

t_BOOL	in_queue(t_queue *queue, int id)
{
	while (queue->next)
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

	neighbour = queue;
	coder = queue;
	while (((t_coder *)neighbour->coder)->id != neighbour_id)
		neighbour = neighbour->next;
	neighbour_coder = (t_coder *)neighbour->coder;
	while (((t_coder *)coder->coder)->id != id)
		coder = coder->next;
	self = (t_coder *)coder->coder;
	if (scheduler == FIFO)
		if (get_total_time_ms(coder->time_in_queue) > get_total_time_ms(neighbour->time_in_queue))
			return (TRUE);
	else if (get_actual_time_ms(self->last_compile) + self->data.burnout_time < get_actual_time(neighbour_coder->last_compile) + neighbour_coder->data.burnout_time)
		return (TRUE);
	return (FALSE);
}

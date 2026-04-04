/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 13:19:47 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/04 14:44:03 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	queue_manipulation(t_coder *coder, t_queue_arg signal, t_dongle *dongle)
{
	t_queue_op	func;

	if (signal == ADD)
		func = add_to_waiting_list;
	else
		func = remove_from_waiting_list;
	func(coder, dongle);
}

void	add_to_waiting_list(t_coder *coder, t_dongle *dongle)
{
	t_queue	*element;
	t_queue	*temp;

	element = malloc(sizeof(t_queue));
	element->coder_id = coder->id;
	element->last_compile = *coder->last_compile;
	element->burnout_time = coder->data.burnout_time;
	element->coder = (t_coder *) coder;
	element->next = NULL;
	pthread_mutex_lock(dongle->queue_mutex);
	if (dongle->waiting_list == NULL)
		dongle->waiting_list = element;
	else
	{
		temp = dongle->waiting_list;
		while (temp->next)
			temp = temp->next;
		temp->next = element;
	}
	pthread_mutex_unlock(dongle->queue_mutex);
}

void	remove_from_waiting_list(t_coder *coder, t_dongle *dongle)
{
	t_queue	*temp;
	t_queue	*previous;

	pthread_mutex_lock(dongle->queue_mutex);
	temp = dongle->waiting_list;
	previous = NULL;
	while (temp && temp->coder_id != coder->id)
	{
		previous = temp;
		temp = temp->next;
	}
	if (previous == NULL)
		dongle->waiting_list = temp->next;
	else
		previous->next = temp->next;
	free(temp);
	pthread_mutex_unlock(dongle->queue_mutex);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:58:56 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/03 20:20:36 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

t_BOOL	priority(t_coder *coder, t_dongle *dongle)
{
	if (no_neighbour_in(coder) == TRUE)
		return (TRUE);
	if (coder->data.scheduler == FIFO)
	{
		if (im_first_in(coder, dongle) == FALSE)
			return (FALSE);
	}
	else
	{
		if(i_will_burn_out(coder, dongle) == FALSE)
			return (FALSE);
	}
	return (TRUE);
}

t_BOOL	no_neighbour_in(t_coder *coder)
{
	t_BOOL	result;
	t_queue	*temp;

	result = TRUE;
	pthread_mutex_lock(coder->left->queue_mutex);
	temp = coder->left->waiting_list;
	while (temp)
	{
		if (temp->coder_id != coder->id)
			result = FALSE;
		temp = temp->next;
	}
	pthread_mutex_unlock(coder->left->queue_mutex);
	pthread_mutex_lock(coder->right->queue_mutex);
	temp = coder->right->waiting_list;
	while (result == TRUE && temp)
	{
		if (temp->coder_id != coder->id)
			result = FALSE;
		temp = temp->next;
	}
	pthread_mutex_unlock(coder->right->queue_mutex);
	return (result);
}

t_BOOL	im_first_in(t_coder *coder, t_dongle *dongle)
{
	t_BOOL	result;

	pthread_mutex_lock(dongle->queue_mutex);
	result = TRUE;
	if (dongle->waiting_list->coder_id != coder->id)
		result = FALSE;
	pthread_mutex_unlock(dongle->queue_mutex);
	return (result);
}

t_BOOL	i_will_burn_out(t_coder *coder, t_dongle *dongle)
{
	t_BOOL	result;
	t_queue	*temp;
	long long my_time;
	long long neighbour_time;

	pthread_mutex_lock(dongle->queue_mutex);
	temp = dongle->waiting_list;
	result = TRUE;
	while (temp->next && temp->coder_id == coder->id)
		temp = temp->next;
	my_time = get_total_time_timeval(*coder->last_compile) + coder->data.burnout_time;
	neighbour_time = get_total_time_timeval(temp->last_compile) + temp->burnout_time;
	if (my_time > neighbour_time)
		result = FALSE;
	pthread_mutex_unlock(dongle->queue_mutex);
	return (result);
}

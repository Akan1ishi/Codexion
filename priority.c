/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:58:56 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/04 14:35:33 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

t_BOOL	i_have_priority(t_coder *coder, t_dongle *dongle)
{
	if (neighbour_not_elligible(coder, dongle) == TRUE)
		return (TRUE);
	return (priority(coder, dongle));
}

t_BOOL	priority(t_coder *coder, t_dongle *dongle)
{
	if (coder->data.scheduler == FIFO)
		return (im_first_in(coder, dongle));
	else
		return (i_will_burn_out(coder, dongle));
}

t_BOOL	neighbour_not_elligible(t_coder *coder, t_dongle *dongle)
{
	t_queue		*temp;
	t_coder		*neighbour;
	t_dongle	*neighbour_dongle;

	pthread_mutex_lock(dongle->queue_mutex);
	temp = dongle->waiting_list;
	neighbour = 0;
	while (temp)
	{
		if (temp->coder_id != coder->id)
			neighbour = (t_coder *) temp->coder;
		temp = temp->next;
	}
	pthread_mutex_unlock(dongle->queue_mutex);
	if (!neighbour)
		return (TRUE);
	if (dongle == coder->left)
		neighbour_dongle = neighbour->left;
	else
		neighbour_dongle = neighbour->right;
	if (priority(neighbour, neighbour_dongle) == TRUE)
		return (FALSE);
	return (TRUE);
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
	t_BOOL		result;
	t_queue		*temp;
	long long	my_time;
	long long	neighbour_time;

	pthread_mutex_lock(dongle->queue_mutex);
	temp = dongle->waiting_list;
	result = TRUE;
	while (temp->next && temp->coder_id == coder->id)
		temp = temp->next;
	my_time = (get_total_time_timeval(*coder->last_compile)
			+ coder->data.burnout_time);
	neighbour_time = (get_total_time_timeval(temp->last_compile)
			+ temp->burnout_time);
	if (my_time > neighbour_time)
		result = FALSE;
	pthread_mutex_unlock(dongle->queue_mutex);
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 17:51:08 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/04 14:25:49 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

struct timespec	*convert_dongle(struct timeval time, struct timespec *new)
{
	struct timespec	converted_time;

	converted_time.tv_sec = time.tv_sec;
	converted_time.tv_nsec = time.tv_usec * 1000;
	if (converted_time.tv_nsec > 1000000000)
	{
		converted_time.tv_sec += converted_time.tv_nsec / 1000000000;
		converted_time.tv_nsec %= 1000000000;
	}
	*new = converted_time;
	return (new);
}

void	unlock_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		coder->right->free = TRUE;
		pthread_cond_broadcast(coder->right->wait);
		pthread_mutex_unlock(coder->right->mutex);
		coder->left->free = TRUE;
		pthread_cond_broadcast(coder->left->wait);
		pthread_mutex_unlock(coder->left->mutex);
	}
	else
	{
		coder->left->free = TRUE;
		pthread_cond_broadcast(coder->left->wait);
		pthread_mutex_unlock(coder->left->mutex);
		coder->right->free = TRUE;
		pthread_cond_broadcast(coder->right->wait);
		pthread_mutex_unlock(coder->right->mutex);
	}
}

void	lock_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timeval	current_time;
	struct timespec	cooldown;

	pthread_mutex_lock(dongle->mutex);
	queue_manipulation(coder, ADD, dongle);
	while (dongle->free == FALSE || i_have_priority(coder, dongle) == FALSE)
		pthread_cond_wait(dongle->wait, dongle->mutex);
	dongle->free = FALSE;
	queue_manipulation(coder, REMOVE, dongle);
	gettimeofday(&current_time, NULL);
	if (get_total_time_timeval(current_time)
		< get_total_time_timeval(dongle->next_free))
		pthread_cond_timedwait(dongle->wait, dongle->mutex,
			convert_dongle(dongle->next_free, &cooldown));
}

void	ask_for_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		lock_dongle(coder, coder->right);
		lock_dongle(coder, coder->left);
	}
	else
	{
		lock_dongle(coder, coder->left);
		lock_dongle(coder, coder->right);
	}
}

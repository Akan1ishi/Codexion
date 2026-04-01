/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 17:51:08 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/01 18:23:38 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

struct timespec	*convert_longest_dongle(t_coder *coder, struct timespec *cd)
{
	struct timespec	converted_time;
	struct timeval	left;
	struct timeval	right;
	struct timeval	highest;

	manage_dongles(coder, LOCK);
	left = coder->left->next_free;
	right = coder->right->next_free;
	if (get_total_time_timeval(left) > get_total_time_timeval(right))
		highest = left;
	else
		highest = right;
	converted_time.tv_sec = highest.tv_sec;
	converted_time.tv_nsec = highest.tv_usec * 1000;
	if (converted_time.tv_nsec > 1000000000)
	{
		converted_time.tv_sec += converted_time.tv_nsec / 1000000000;
		converted_time.tv_nsec %= 1000000000;
	}
	*cd = converted_time;
	manage_dongles(coder, UNLOCK);
	return (cd);
}

void	manage_dongles(t_coder *coder, t_signal signal)
{
	t_mutex_op	mutex_handler;

	mutex_handler = get_mutex_op(signal);
	if (coder->id % 2 == 0)
	{
		mutex_handler(coder->left->mutex);
		mutex_handler(coder->right->mutex);
	}
	else
	{
		mutex_handler(coder->right->mutex);
		mutex_handler(coder->left->mutex);
	}
}

void	inscribe_dongle_data(t_coder *coder)
{
	struct timeval	tz;

	gettimeofday(&tz, NULL);
	pthread_mutex_lock(coder->burnout_mutex);
	*coder->last_compile = tz;
	manage_dongles(coder, LOCK);
	pthread_mutex_unlock(coder->burnout_mutex);
	tz.tv_sec += coder->data.dongle_time / 1000;
	tz.tv_usec += (coder->data.dongle_time * 1000) % 1000000;
	coder->right->next_free = tz;
	coder->left->next_free = tz;
	coder->right->free = TRUE;
	coder->left->free = TRUE;
	manage_dongles(coder, UNLOCK);
	signal_neighbours(coder);
}

void	signal_neighbours(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(coder->left_mutex);
		pthread_cond_signal(coder->left_cond);
		pthread_mutex_unlock(coder->left_mutex);
		pthread_mutex_lock(coder->right_mutex);
		pthread_cond_signal(coder->right_cond);
		pthread_mutex_unlock(coder->right_mutex);
	}
	else
	{
		pthread_mutex_lock(coder->right_mutex);
		pthread_cond_signal(coder->right_cond);
		pthread_mutex_unlock(coder->right_mutex);
		pthread_mutex_lock(coder->left_mutex);
		pthread_cond_signal(coder->left_cond);
		pthread_mutex_unlock(coder->left_mutex);
	}
}

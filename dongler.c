/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 17:51:08 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/31 19:42:22 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

t_BOOL	dongles_are_free(t_coder *coder)
{
	t_BOOL	result;

	result = TRUE;
	lock_dongles(coder, LOCK);
	if (coder->right->free == FALSE || coder->left->free == FALSE)
		result = FALSE;
	lock_dongles(coder, UNLOCK);
	if (dongles_on_cooldown(coder) == TRUE)
		result = FALSE;
	return (result);
}

t_BOOL	dongles_on_cooldown(t_coder *coder)
{
	struct timeval	current_time;
	long long		total_current_time;
	t_BOOL			result;

	result = FALSE;
	gettimeofday(&current_time, NULL);
	total_current_time = get_total_time_timeval(current_time);
	lock_dongles(coder, LOCK);
	if (get_total_time_timeval(coder->left->next_free) > total_current_time
		|| get_total_time_timeval(coder->right->next_free) > total_current_time)
		result = TRUE;
	lock_dongles(coder, UNLOCK);
	return (result);
}

struct timespec	*convert_longest_dongle(t_coder *coder, struct timespec *cd)
{
	struct timespec	converted_time;
	struct timeval	left;
	struct timeval	right;
	struct timeval	highest;

	lock_dongles(coder, LOCK);
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
	lock_dongles(coder, UNLOCK);
	return (cd);
}

void	manage_dongles(t_coder *coder, t_signal signal)
{
	t_mutex_op	mutex_handler;

	mutex_handler = get_mutex_op(signal);
	if (coder->id % 2 == 0)
	{
		mutex_handler(&coder->left->mutex);
		mutex_handler(&coder->right->mutex);
	}
	else
	{
		mutex_handler(&coder->right->mutex);
		mutex_handler(&coder->left->mutex);
	}
	if (signal == LOCK)
	{
		lock_dongles(coder, LOCK);
		coder->right->free = FALSE;
		coder->left->free = FALSE;
		lock_dongles(coder, UNLOCK);
	}
}

void	inscribe_dongle_data(t_coder *coder)
{
	struct timeval	tz;

	gettimeofday(&tz, NULL);
	pthread_mutex_lock(&coder->burnout_mutex);
	*coder->last_compile = tz;
	pthread_mutex_unlock(&coder->burnout_mutex);
	lock_dongles(coder, LOCK);
	tz.tv_sec += coder->data.dongle_time / 1000;
	tz.tv_usec += (coder->data.dongle_time * 1000) % 1000000;
	coder->right->next_free = tz;
	coder->left->next_free = tz;
	coder->right->free = TRUE;
	coder->left->free = TRUE;
	lock_dongles(coder, UNLOCK);
}

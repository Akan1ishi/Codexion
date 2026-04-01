/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_queue.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 14:23:33 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/01 18:17:44 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	wait_on_dongles(t_coder *coder)
{
		struct timespec	time;

		pthread_mutex_lock(coder->wait_mutex);
		while (dongles_are_free(coder) == FALSE || priority(coder) == FALSE)
		{
			if (dongles_on_cooldown(coder) == TRUE)
			{
				get_dongle_cooldown(coder, &time);
				pthread_cond_timedwait(coder->wait_cond, coder->wait_mutex, &time);
			}
			else
				pthread_cond_wait(coder->wait_cond, coder->wait_mutex);
		}
		pthread_mutex_unlock(coder->wait_mutex);
}

t_dongle_type	get_dongle_cooldown(t_coder *coder, struct timespec *time)
{
	struct timespec	converted_time;
	struct timeval	left;
	struct timeval	right;
	struct timeval	highest;
	t_dongle_type	type;

	manage_dongles(coder, LOCK);
	left = coder->left->next_free;
	right = coder->right->next_free;
	if (get_total_time_timeval(left) >= get_total_time_timeval(right))
	{
		highest = left;
		type = LEFT;
	}
	else
	{
		highest = right;
		type = RIGHT;
	}
	manage_dongles(coder, UNLOCK);
	converted_time.tv_sec = highest.tv_sec;
	converted_time.tv_nsec = highest.tv_usec * 1000;
	if (converted_time.tv_nsec > 1000000000)
	{
		converted_time.tv_sec += converted_time.tv_nsec / 1000000000;
		converted_time.tv_nsec %= 1000000000;
	}
	*time = converted_time;
	return (type);
}

t_BOOL	dongles_are_free(t_coder *coder)
{
	t_BOOL	result;

	result = TRUE;
	manage_dongles(coder, LOCK);
	if (coder->right->free == FALSE || coder->left->free == FALSE)
		result = FALSE;
	manage_dongles(coder, UNLOCK);
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
	manage_dongles(coder, LOCK);
	if (get_total_time_timeval(coder->left->next_free) > total_current_time
		|| get_total_time_timeval(coder->right->next_free) > total_current_time)
		result = TRUE;
	manage_dongles(coder, UNLOCK);
	return (result);
}

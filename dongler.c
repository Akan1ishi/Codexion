/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 17:51:08 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/18 11:38:31 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

t_BOOL	dongles_are_free(t_coder *coder)
{
	if (coder->right->free == FALSE || coder->left->free == FALSE)
		return FALSE;
	if (dongles_on_cooldown(coder) == TRUE)
		return FALSE;
	return (TRUE);
}

t_BOOL	dongles_on_cooldown(t_coder *coder)
{
	struct timeval	current_time;
	long long	total_current_time;

	gettimeofday(&current_time, NULL);
	total_current_time = get_total_time_timeval(current_time);
	if (get_total_time_timeval(coder->left->next_free) > total_current_time ||
			get_total_time_timeval(coder->right->next_free) > total_current_time)
		return (TRUE);
	return (FALSE);
}

struct timespec	*convert_longest_dongle(t_coder *coder, struct timespec *cooldown)
{
	struct timespec	converted_time;
	struct timeval	left;
	struct timeval	right;
	struct timeval	highest;

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
	*cooldown = converted_time;
	return (cooldown);
}

void	manage_dongles(t_coder *coder, t_signal signal)
{
	mutex_op	mutex_handler;

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
		coder->right->free = FALSE;
		coder->left->free = FALSE;
	}
}

void	inscribe_dongle_data(t_coder *coder)
{
	struct timeval	tz;

	gettimeofday(&tz, NULL);
	*coder->last_compile = tz;
	tz.tv_sec += coder->data.dongle_cooldown / 1000;
	tz.tv_usec += (coder->data.dongle_cooldown * 1000) % 1000000;
	coder->right->next_free = tz;
	coder->left->next_free = tz;
	coder->right->free = TRUE;
	coder->left->free = TRUE;
}

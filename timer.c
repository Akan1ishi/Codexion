/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 14:26:51 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/29 15:39:22 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

long long	get_total_time_timeval(struct timeval time)
{
	return ((time.tv_sec * 1000000) + time.tv_usec);
}

t_BOOL	is_burned_out(struct timeval time, struct timeval old, int burnout_time)
{
	long long	ms_difference;

	ms_difference = get_actual_time_ms(old, time);
	if (ms_difference >= burnout_time)
		return (TRUE);
	return (FALSE);
}

void	*end(struct timeval now, t_end signal, int id, t_control *controller)
{
	long long	time;

	*controller->active = FALSE;
	time = get_actual_time_ms(controller->start_time, now);
	pthread_mutex_lock(&controller->output_mutex);
	if (signal == SUCCESS)
		printf("%lld coders did their job without fail\n", time);
	else
		printf("%lld %d burned out\n", time, id);
	pthread_mutex_unlock(&controller->output_mutex);
	return (NULL);
}

long long	get_actual_time_ms(struct timeval start, struct timeval now)
{
	long long	seconds;
	long long	u_seconds;

	seconds = now.tv_sec - start.tv_sec;
	u_seconds = now.tv_usec - start.tv_usec;
	if (u_seconds < 0)
	{
		seconds -= 1;
		u_seconds += 1000000;
	}
	return (seconds * 1000 + u_seconds / 1000);
}

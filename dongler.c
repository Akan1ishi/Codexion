/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 17:51:08 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/03 18:36:42 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	inscribe_dongle_data(t_coder *coder)
{
	struct timeval	tz;

	gettimeofday(&tz, NULL);
	pthread_mutex_lock(coder->burnout_mutex);
	*coder->last_compile = tz;
	pthread_mutex_unlock(coder->burnout_mutex);
	tz.tv_sec += coder->data.dongle_time / 1000;
	tz.tv_usec += (coder->data.dongle_time * 1000) % 1000000;
	coder->right->next_free = tz;
	coder->left->next_free = tz;
	coder->right->free = TRUE;
	coder->left->free = TRUE;
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 17:51:08 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/17 19:03:42 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "coders/codexion.h"

void	*run_dongle(void * dongle)
{
	struct timeval	tv;
	t_dongle	self;

	self = *(t_dongle *) dongle;
	pthread_mutex_init(&self.mutex, NULL);
	int i = 0;
	while (TRUE)
	{
		pthread_mutex_lock(&self.mutex);
		//pthread_cond_wait(&self.active, &self.mutex);
		gettimeofday(&tv, NULL);
		//tv.tv_sec += self.cooldown / 1000;
		printf("%lu\n", tv.tv_sec);
		//pthread_cond_timedwait(&self.active, &self.mutex, (struct timespec *) &tv);
		pthread_mutex_unlock(&self.mutex);
		i++;
		if (i == 20)
			break;
		usleep(300);
	}
	return (NULL);
}

int main()
{
	t_dongle	dongle;

	pthread_create(&dongle.id, NULL, run_dongle, (void *) &dongle);
	for (int i = 0; i < 10; i++)
	{
		usleep(1000);
		pthread_cond_broadcast(&dongle.active);
	}
	pthread_join(dongle.id, NULL);
	return (0);	
}

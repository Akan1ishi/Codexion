/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 11:43:07 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/18 16:03:41 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

t_BOOL	check_dongles(t_coder coder)
{
	if (coder.id % 2 == 0)
		if (coder.left->free == FALSE || coder.right->free == FALSE)
			return FALSE;
	else
		if (coder.right->free == FALSE || coder.left->free == FALSE)
			return FALSE;
	return TRUE;

}

void	reserve_dongles(t_coder coder)
{
	if (coder.id % 2 == 0)
	{
		pthread_mutex_lock(&coder.left->mutex);
		pthread_mutex_lock(&coder.right->mutex);
	}
	else
	{
		pthread_mutex_lock(&coder.right->mutex);
		pthread_mutex_lock(&coder.left->mutex);
	}
	coder.left->free = FALSE;
	coder.right->free = FALSE;
}


void	unlock_dongles(t_coder coder)
{
	coder.left->free = TRUE;
	coder.right->free = TRUE;
	pthread_cond_signal(&coder.left_neighbour->wait);
	pthread_cond_signal(&coder.right_neighbour->wait);
	pthread_mutex_unlock(&coder.left->mutex);
	pthread_mutex_unlock(&coder.right->mutex);
}

void	*timer(void *arg)
{
	t_control	*controller;
	struct timezone	tz;

	controller = (t_control *) arg;
	while (TRUE)
	{
		gettimeofday(&tz, NULL);
		&controller->time = tz * 1000;
	}
	return NULL;
}
void	*compile_checker(void *arg)
{
	t_control	*controller;

	controller = (t_control *) arg;
	while (TRUE)
	{
		if (controller->active == FALSE)
			break;
		if (controller->nb_compiles >= controller.data.compile_goal)
		{
			controller->active = FALSE;
			printf("Coders win again");
			break;
		}
	}
	return NULL;
}
void	*burnout_checker(void *arg)
{
	t_control	*controller;
	int		i;

	controller = (t_control *) arg;
	while (TRUE)
	{
		if (controller->active == FALSE)
			break;
		i = 0;
		while (i < controller->data.coders)
		{
			if (controller->coders[i].last_compile - controller.start_time >= controller->data.burnout_time)
			{
				controller->active = FALSE;
				printf("coder %d burned out.", controller->coders[i].id + 1);
				break;
			}
			i++;
		}
	}
	return NULL;
}

void	print_info(t_coder self, t_type type)
{
	if (*&self.active == FALSE)
		return ;
	if (type == COMPILING)
		printf("[%d] coder %d is compiling\n", &coder.time - coder.start_time, coder.id);
	else if (type == REFACTORING)
		printf("[%d] coder %d is refactoring\n", &coder.time - coder.start_time, coder.id);
	else
		printf("[%d] coder %d is debugging\n", &coder.time - coder.start_time, coder.id);
}

void	*coder_life(void * arg
{
	t_coder	self;

	self = *(t_coder *)arg;
	while (TRUE)
	{
		while (check_dongles(self) == FALSE)
			usleep(10);
		reserve_dongles(self);
		print_info(self, COMPILING);
		wait(time);
		unlock_dongles(self);
		print_info(self, REFACTORING);
		wait(time);
		print_info(self, DEBUGGING);
		wait(time);
	}
	return NULL;	
}


void	init_coders(t_control *controller)
{
		
}

void	generate_dongles(t_control *controller)
{
	t_dongle	*dongles;
	int			i;
	
	dongles = malloc(sizeof(t_dongle) * controller->data.coders);
	if (!dongles)
		return ;
	i = 0;
	while (i < controller->data.coders)
	{
		dongles[i].id = i;
		dongles[i].cooldown = controller->data.dongle_time;
		pthread_mutex_init(&dongles[i].mutex, NULL);
		pthread_cond_init(&dongles[i].active, NULL);
		dongles[i].taken = FALSE;
		dongles[i].free = TRUE;
		i++;
	}
	controller->dongles = dongles;
}

void	generate_coders(t_control *controller)
{
	t_coder	*coders;
	int			i;

	coders = malloc(sizeof(t_coder) * controller->data.coders);
	if (!coders)
		return ;
	i = 0;
	while (i < controller->data.coders)
	{
		coders[i].compile_goal = &controller->goal;
		coders[i].id = i;
		coders[i].last_compile = 0;
		coders[i].starved = FALSE;
		coders[i].run = coder_life;
		//coders[i].print = some_func;
		coders[i].work = &controller->nb_compiles;
		coders[i].right = &controller->dongles[i];
		if (i == 0)
			coders[i].left = &controller->dongles[controller->data.coders - 1];
		else
			coders[i].left = &controller->dongles[i - 1];

		i++;
	}
	controller->coders = coders;
}

int main(void)
{
	t_data	test;

	test.coders = 5;
	test.burnout_time = 10000;
	test.compile_time= 10000;
	test.debug_time= 10000;
	test.refactor_time= 10000;
	test.compile_goal = 20;
	test.dongle_time = 5000;
	test.scheduler = FIFO;

	t_control	controller;
	controller.nb_compiles = 0;
	controller.data = test;
	generate_dongles(&controller);
	generate_coders(&controller);
	for (int i = 0; i < test.coders; i++)
	{
		printf("Coder ID: %d, Left dongle ID: %d, Right dongle ID: %d\n",
						controller.coders[i].id,
						controller.coders[i].left->id,
						controller.coders[i].right->id
						);
	}
	for (int i = 0; i < controller.data.coders; i++)
		pthread_create(&controller.coders[i].thread, NULL,
						coder_life, &controller.coders[i]);
	for (int i = 0; i < controller.data.coders; i++)
		pthread_join(controller.coders[i].thread, NULL);

	return 0;
}

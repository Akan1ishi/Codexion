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

void	*coder_life(void * arg)
{
	t_coder	self;

	self = *(t_coder *)arg;
	while (TRUE)
	{		
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

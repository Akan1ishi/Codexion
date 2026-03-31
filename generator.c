/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 11:43:07 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/29 17:26:11 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	generate_dongles(t_control *controller)
{
	t_dongle		*dongles;
	unsigned int	i;

	dongles = malloc(sizeof(t_dongle) * controller->data.coders);
	if (!dongles)
		return ;
	i = 0;
	while (i < controller->data.coders)
	{
		pthread_mutex_init(&dongles[i].mutex, NULL);
		dongles[i].free = TRUE;
		dongles[i].next_free = controller->start_time;
		i++;
	}
	controller->dongles = dongles;
}

void	generate_coders(t_control *controller)
{
	t_coder			*coders;
	unsigned int	i;

	coders = malloc(sizeof(t_coder) * controller->data.coders);
	if (!coders)
		return ;
	i = 0;
	while (i < controller->data.coders)
	{
		pthread_mutex_init(&coders[i].burnout_mutex, NULL);
		coders[i].data = controller->data;
		coders[i].nb_compiles = controller->nb_compiles;
		coders[i].start_time = controller->start_time;
		coders[i].id = i + 1;
		coders[i].last_compile = malloc(sizeof(struct timeval));
		*coders[i].last_compile = controller->start_time;
		coders[i].queue_mutex = &controller->queue_mutex;
		coders[i].output_mutex = &controller->output_mutex;
		coders[i].queue_cond = &controller->queue_cond;
		coders[i].work_mutex = &controller->work_mutex;
		coders[i].queue = &controller->queue;
		assign_dongle(coders, controller, i);
		i++;
	}
	controller->coders = coders;
}

t_control	init_controller(t_data data)
{
	t_control	controller;

	controller.data = data;
	controller.nb_compiles = malloc(sizeof(int));
	*controller.nb_compiles = 0;
	controller.thread = 0;
	controller.active = malloc(sizeof(t_BOOL));
	*controller.active = TRUE;
	controller.queue = NULL;
	pthread_mutex_init(&controller.queue_mutex, NULL);
	pthread_cond_init(&controller.queue_cond, NULL);
	pthread_mutex_init(&controller.output_mutex, NULL);
	pthread_mutex_init(&controller.work_mutex, NULL);
	gettimeofday(&controller.start_time, NULL);
	generate_dongles(&controller);
	generate_coders(&controller);
	return (controller);
}

void	start_threads(t_control controller)
{
	unsigned int	i;

	pthread_create(&controller.thread, NULL, surveil, (void *) &controller);
	i = 0;
	while (i < controller.data.coders)
	{
		pthread_create(&controller.coders[i].thread, NULL,
			code, (void *) &controller.coders[i]);
		i++;
	}
	i = 0;
	pthread_join(controller.thread, NULL);
	while (i < controller.data.coders)
	{
		pthread_join(controller.coders[i].thread, NULL);
		i++;
	}
}

void	assign_dongle(t_coder *coders, t_control *controller, unsigned int id)
{
	coders[id].active = controller->active;
	coders[id].right = &controller->dongles[id];
	if (id == 0)
		coders[id].left = &controller->dongles[controller->data.coders - 1];
	else
		coders[id].left = &controller->dongles[id - 1];
}

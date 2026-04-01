/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 11:43:07 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/01 11:38:45 by lumarcuc         ###   ########.fr       */
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
		dongles[i].mutex = malloc(sizeof(pthread_mutex_t));
		dongles[i].data_mutex = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(dongles[i].mutex, NULL);
		pthread_mutex_init(dongles[i].data_mutex, NULL);
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
		coders[i].data = controller->data;
		coders[i].nb_compiles = 0;
		coders[i].start_time = controller->start_time;
		coders[i].id = i + 1;
		coders[i].last_compile = malloc(sizeof(struct timeval));
		*coders[i].last_compile = controller->start_time;
		coders[i].queue_mutex = controller->queue_mutex;
		coders[i].active_mutex = controller->active_mutex;
		coders[i].output_mutex = controller->output_mutex;
		coders[i].wait_cond = controller->wait_cond;
		coders[i].wait_mutex = controller->wait_mutex;
		coders[i].finished = FALSE;
		assign_dongle(coders, controller, i);
		i++;
	}
	controller->coders = coders;
}

t_control	init_controller(t_data data)
{
	t_control	controller;

	controller.data = data;
	controller.active = malloc(sizeof(t_BOOL));
	*controller.active = TRUE;
	controller.queue = NULL;
	controller.queue_mutex = malloc(sizeof(pthread_mutex_t));
	controller.output_mutex = malloc(sizeof(pthread_mutex_t));
	controller.active_mutex = malloc(sizeof(pthread_mutex_t));
	controller.wait_cond = malloc(sizeof(pthread_cond_t));
	controller.wait_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(controller.queue_mutex, NULL);
	pthread_mutex_init(controller.active_mutex, NULL);
	pthread_cond_init(controller.wait_cond, NULL);
	pthread_mutex_init(controller.output_mutex, NULL);
	pthread_mutex_init(controller.wait_mutex, NULL);
	gettimeofday(&controller.start_time, NULL);
	generate_dongles(&controller);
	generate_coders(&controller);
	return (controller);
}

void	start_threads(t_control controller)
{
	unsigned int	i;
	void			*coder_func;

	pthread_create(&controller.thread, NULL, surveil, (void *) &controller);
	coder_func = code;
	if (controller.data.coders == 1)
		coder_func = rot_in_hell;
	i = 0;
	while (i < controller.data.coders)
	{
		pthread_create(&controller.coders[i].thread, NULL,
			coder_func, (void *) &controller.coders[i]);
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
	coders[id].burnout_mutex = malloc(sizeof(pthread_mutex_t));
	coders[id].compile_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(coders[id].burnout_mutex, NULL);
	pthread_mutex_init(coders[id].compile_mutex, NULL);
	coders[id].active = controller->active;
	coders[id].queue = &controller->queue;
	coders[id].right = &controller->dongles[id];
	if (id == 0)
		coders[id].left = &controller->dongles[controller->data.coders - 1];
	else
		coders[id].left = &controller->dongles[id - 1];
}

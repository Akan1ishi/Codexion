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
		pthread_mutex_init(&dongles[i].mutex, NULL);
		dongles[i].free = TRUE;
		dongles[i].next_free = controller->start_time;
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
		coders[i].data.compile_time = controller->data.compile_time;
		coders[i].data.debug_time = controller->data.debug_time;
		coders[i].data.refactor_time = controller->data.refactor_time;
		coders[i].data.dongle_cooldown = controller->data.dongle_time;
		coders[i].data.total_coders = controller->data.coders;
		coders[i].data.scheduler = controller->data.scheduler;
		coders[i].data.burnout_time = controller->data.burnout_time;
		coders[i].nb_compiles = controller->nb_compiles;
		coders[i].start_time = controller->start_time;
		coders[i].id = i + 1;
		coders[i].last_compile = controller->start_time;
		coders[i].right = &controller->dongles[i];
		coders[i].global_mutex = &controller->global_mutex;
		coders[i].output_mutex = &controller->output_mutex;
		coders[i].global_cond = &controller->global_cond;
		coders[i].queue = controller->queue;
		if (i == 0)
			coders[i].left = &controller->dongles[controller->data.coders - 1];
		else
			coders[i].left = &controller->dongles[i - 1];

		i++;
	}
	controller->coders = coders;
}

t_control	init_controller(t_data data)
{
	t_control	controller;

	controller.data = data;
	*controller.nb_compiles = 0;
	controller.queue = NULL;
	pthread_mutex_init(&controller.global_mutex, NULL);
	pthread_mutex_init(&controller.output_mutex, NULL);
	pthread_cond_init(&controller.global_cond, NULL);
	gettimeofday(&controller.start_time, NULL);
	generate_dongles(&controller);
	generate_coders(&controller);
	return (controller);
}

void	start_threads(t_control controller)
{
	int	i;

	pthread_create(&controller.thread, NULL, test_surveil, (void *) &controller);
	i = 0;
	while (i < controller.data.coders)
	{
		pthread_create(&controller.coders[i].thread, NULL, test_code, (void *) &controller.coders[i]);
	i++;
	}
	pthread_join(controller.thread, NULL);
	i = 0;
	while (i < controller.data.coders)
		pthread_join(controller.coders[i++].thread, NULL);
}

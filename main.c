/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                   +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:07:37 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/18 11:39:20 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

int	main(int ac, char **av)
{
	t_data	data;
	t_control	controller;
	data.coders = 3;
	data.burnout_time = 5000;
	data.compile_time = 250;
	data.debug_time = 500;
	data.refactor_time = 350;
	data.compile_goal = 100;
	data.dongle_time = 300;
	data.scheduler = FIFO;

	controller = init_controller(data);
	start_threads(controller);
	return (0);
}

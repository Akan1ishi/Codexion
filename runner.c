/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 11:39:57 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/18 11:42:34 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	run_stuff(t_data data)
{	
	t_control	controller;

	controller.data = data;
	controller.dongles = generate_dongles(controller);
	controller.coders = generate_coders(controller);
}

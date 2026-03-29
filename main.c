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

	if (ft_validate_input(ac, av) == FALSE)
		return (1);
	data = convert_input_to_data(ac - 1, &av[1]);
	controller = init_controller(data);
	start_threads(controller);
	free_everything(&controller);
	return (0);
}

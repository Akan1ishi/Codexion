/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                   +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:07:37 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/12 17:48:04 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	*print_yo(void *info)
{
	printf("%s\n", (char *) info);
	return NULL;
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ft_validate_input(ac, av) == false)
		return (printf("Wrong Input"));
	data = convert_input_to_data(ac - 1, &av[1]);
	printf("%d, %d, %d, %d , %d ,%d, %d, %u\n",
			data.coders,
			data.burnout_time,
			data.compile_time,
			data.debug_time,
			data.refactor_time,
			data.compile_goal,
			data.dongle_time,
			data.scheduler);
	pthread_t	yo;
	pthread_t	ya;
	char	*test;
	test = "Salut ca va\n";
	pthread_create(&yo, NULL, print_yo, (void *)test);
	pthread_create(&ya, NULL, print_yo, (void *)test);
	pthread_join(yo, NULL);
	return 0;
}

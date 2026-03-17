/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:07:43 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/12 18:11:20 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

bool	check_int(char *str)
{
	size_t	i;
	size_t	len;

	len = strlen(str);
	i = 0;
	while (i < len)
	{
		if (!(str[i] >= '0' || str[i] <= '9'))
			return false;
		i++;
	}
	return true;
}

int	check_str(char *str)
{
	char	*fifo;
	char	*edf;

	fifo = "fifo";
	edf = "edf";
	if (str[0] == 'f')
	{
		if (strcmp(str, fifo) == false)
			return (EDF);
	}
	else if (str[0] == 'e')
	{
		if (strcmp(str, edf) == false)
			return (FIFO);
	}
	return (false);
}

bool	ft_validate_input(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 9)
		return (false);
	while (i < 8)
		if (check_int(av[i++]) == false)
			return (false);
	if (check_str(av[i]) == false)
		return false;
	return (true);
}

t_data	convert_input_to_data(int ac, char **av)
{
	t_data	data;
	int	i;

	i = 0;
	while (i < ac)
	{
		if (i == 0)
			data.coders = atoi(av[i]);
		if (i == 1)
			data.burnout_time = atoi(av[i]);
		if (i == 2)
			data.compile_time = atoi(av[i]);
		if (i == 3)
			data.debug_time = atoi(av[i]);
		if (i == 4)
			data.refactor_time = atoi(av[i]);
		if (i == 5)
			data.compile_goal = atoi(av[i]);
		if (i == 6)
			data.dongle_time = atoi(av[i]);
		if (i == 7)
			data.scheduler = check_str(av[i]);
		i++;
	}
	return (data);
}

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
	return (true);
}

bool	check_ms(char *str)
{
	return (true);
}

bool	check_str(char *str)
{
	char	*fifo;
	char	*edf;

	fifo = "fifo";
	edf = "edf";
	if (str[0] == 'f')
	{
		if (strcmp(str, fifo) == false)
			return (false);
	}
	else if (str[0] == 'e')
	{
		if (strcmp(str, edf) == false)
			return (false);
	}
	else
		return (false);
	return (true);
}

bool	ft_parse(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 9)
		return (false);
	if (check_int(av[i++]) == false)
		return (false);
	while (i < 6)
		if (check_ms(av[i++]) == false)
			return (false);
	if (check_int(av[i++]) == false)
		return (false);
	if (check_ms(av[i++]) == false)
		return (false);
	if (check_str(av[i]) == false)
		return (false);
	return (true);
}

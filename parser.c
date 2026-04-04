/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 17:07:43 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/04/04 13:31:49 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

t_BOOL	check_int(char *str)
{
	size_t	i;
	size_t	len;

	len = strlen(str);
	i = 0;
	while (i < len)
	{
		if ((str[i] >= '0' && str[i] <= '9') || (i == 0 && str[i] == '+'))
			i++;
		else
			return (FALSE);
	}
	if (len > 10)
		return (FALSE);
	if (len == 10)
	{
		if (strcmp(str, "2147483647") > 0)
			return (FALSE);
	}
	return (TRUE);
}

int	check_str(char *str)
{
	char	*fifo;
	char	*edf;

	fifo = "fifo";
	edf = "edf";
	if (str[0] == 'f')
	{
		if (strcmp(str, fifo) == 0)
			return (FIFO);
	}
	else if (str[0] == 'e')
	{
		if (strcmp(str, edf) == 0)
			return (EDF);
	}
	return (FALSE);
}

t_BOOL	ft_validate_input(int ac, char **av)
{
	int		i;
	int		nb_errors;
	t_BOOL	error;

	error = TRUE;
	i = 1;
	nb_errors = 0;
	if (ac != 9)
		return (log_error(&nb_errors, ARGS, 0, NULL));
	while (i < 8)
	{
		if (check_int(av[i]) == FALSE)
			error = log_error(&nb_errors, INT, i, av[i]);
		i++;
	}
	if (check_str(av[i]) == FALSE)
		error = log_error(&nb_errors, SCHEDULE, i, av[i]);
	return (error);
}

t_data	convert_input_to_data(int ac, char **av)
{
	t_data	data;
	int		i;

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

t_BOOL	log_error(int *nb_error, t_log type, int arg_index, char *arg)
{
	*nb_error += 1;
	if (*nb_error == 1)
		printf("[ERROR] Wrong input\n");
	if (type == ARGS)
		printf("\nIncorrect number of arguments ; has to be exactly 8\n");
	if (type == INT)
		printf("\n[ARG %d] '%s' : argument is not a valid int ; "
			"has to be between 0 and INT_MAX included ; "
			"no negatives allowed\n", arg_index, arg);
	if (type == SCHEDULE)
		printf("\n[ARG %d] '%s' : argument is not a valid scheduler ; "
			"has to be exactly one of 'fifo' or 'edf'\n", arg_index, arg);
	return (FALSE);
}

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

int	main(int ac, char **av)
{
	struct timeval	tv;

	settimeofday(&tv, 0);
	ft_printf("%d", tv.tv_usec / 60 / 60);
	av[0][0] = ac;
}

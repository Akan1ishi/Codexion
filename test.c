/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumarcuc <lumarcuc@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 15:39:05 by lumarcuc          #+#    #+#             */
/*   Updated: 2026/03/31 18:41:18 by lumarcuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

t_BOOL	everyone_is_finished(t_control *controller)
{
	unsigned int	i;

	i = 0;
	while (i < controller->data.coders)
	{
		if (is_finished(&controller->coders[i]) == FALSE)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

t_BOOL	is_finished(t_coder *coder)
{
	t_BOOL	result;

	result = FALSE;
	pthread_mutex_lock(&coder->compile_mutex);
	if (coder->finished == TRUE)
		result = TRUE;
	pthread_mutex_unlock(&coder->compile_mutex);
	return (result);
}

t_BOOL	supervisor_said_its_over(t_coder *coder)
{
	t_BOOL	result;

	result = FALSE;
	pthread_mutex_lock(coder->active_mutex);
	if (*coder->active == FALSE)
		result = TRUE;
	pthread_mutex_unlock(coder->active_mutex);
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/25 21:04:30 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(char **argv)
{
	int		status;
	size_t	i;

	status = 0;
	i = 1;
	while (argv[i])
	{
		if (map_unset(envmap, argv[i]) < 0)
		{
			builtin_error("unset", argv[i], "not a valid identifier");
			status = 1;
		}
		else
			status = 0;
		i++;
	}
	return (status);
}

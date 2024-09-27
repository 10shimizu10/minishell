/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 17:23:52 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **argv, t_shell *shell)
{
	t_item	*cur;

	(void)argv;
	cur = shell->envmap->item_head.next;
	while (cur)
	{
		if (cur->value)
		{
			write(STDOUT_FILENO, cur->name, strlen(cur->name));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, cur->value, strlen(cur->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		cur = cur->next;
	}
	write(STDOUT_FILENO, "_=/usr/bin/env\n", 15);
	return (0);
}
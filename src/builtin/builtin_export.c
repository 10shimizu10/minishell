/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 17:21:16 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_allenv(t_shell *shell)
{
	t_item	*cur;

	cur = shell->envmap->item_head.next;
	while (cur)
	{
		write(1, "declare -x ", 11);
		write(1, cur->name, strlen(cur->name));
		if (cur->value)
		{
			write(1, "=\"", 2);
			write(1, cur->value, strlen(cur->value));
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		cur = cur->next;
	}
}

int	builtin_export(char **argv, t_shell *shell)
{
	size_t	i;
	int		status;

	if (argv[1] == NULL)
	{
		print_allenv(shell);
		return (0);
	}
	status = 0;
	i = 1;
	while (argv[i])
	{
		if (map_put(shell->envmap, argv[i], true) < 0)
		{
			builtin_error("export", argv[i], "not a valid identifier");
			status = 1;
		}
		i++;
	}
	return (status);
}

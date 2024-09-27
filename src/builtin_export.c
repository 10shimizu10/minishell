/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 13:06:40 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_allenv(t_shell *shell)
{
	t_item	*cur;

	cur = shell->envmap->item_head.next;
	while (cur)
	{
		if (cur->value)
			ft_printf("declare -x %s=\"%s\"\n", cur->name, cur->value);
		else
			ft_printf("declare -x %s\n", cur->name);
		cur = cur->next;
	}
}

int	builtin_export(char **argv, t_shell *shell)
{
	size_t i;
	int status;

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
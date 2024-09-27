/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 13:04:48 by a.               ###   ########.fr       */
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
			ft_printf("%s=%s\n", cur->name, cur->value);
		cur = cur->next;
	}
	ft_printf("_=/usr/bin/env\n");
	return (0);
}

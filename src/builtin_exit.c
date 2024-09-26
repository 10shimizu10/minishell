/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 00:51:43 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_numeric(char *s)
{
	if (!is_digit(*s))
		return (false);
	while (*s)
	{
		if (!is_digit(*s))
			return (false);
		s++;
	}
	return (true);
}

int builtin_exit(char **argv, t_shell *shell)
{
	long res;
	char *arg;
	char *endptr;

	if (argv[1] == NULL)
		exit(shell->last_status);  // シェルの終了ステータスで終了
	if (argv[2])
	{
		xperror("exit: too many arguments");
		return (1);
	}
	arg = argv[1];
	if (is_numeric(arg))
	{
		errno = 0;
		res = ft_strtol(arg, &endptr, 10);
		if (errno == 0 && *endptr == '\0')
			exit((int)res);  // 数値引数が有効な場合、そのステータスで終了
	}
	xperror("exit: numeric argument required");
	exit(255);  // 数字以外の引数が渡された場合は255で終了
}
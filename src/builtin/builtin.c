/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 17:20:52 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtin(char **argv, t_shell *shell)
{
	if (ft_strcmp(argv[0], "exit") == 0)
		return (builtin_exit(argv, shell));
	if (ft_strcmp(argv[0], "export") == 0)
		return (builtin_export(argv, shell));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (builtin_unset(argv, shell));
	if (ft_strcmp(argv[0], "env") == 0)
		return (builtin_env(argv, shell));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (builtin_cd(argv, shell));
	if (ft_strcmp(argv[0], "echo") == 0)
		return (builtin_echo(argv));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (builtin_pwd(argv, shell));
	todo("exec_builtin");
	return (1);
}

int	exec_builtin(t_node *node, t_shell *shell)
{
	int		status;
	char	**argv;

	do_redirect(node->command->redirects);
	argv = token_list_to_argv(node->command->args);
	status = handle_builtin(argv, shell);
	free_argv(argv);
	reset_redirect(node->command->redirects);
	return (status);
}

bool	is_builtin(t_node *node)
{
	const char		*cmd_name;
	char			*builtin_commands[] = {"exit", "export", "unset", "env",
					"cd", "echo", "pwd"};
	unsigned int	i;

	if (node == NULL || node->command == NULL | node->command->args == NULL
		|| node->command->args->word == NULL)
		return (false);
	cmd_name = node->command->args->word;
	i = 0;
	while (i < sizeof(builtin_commands) / sizeof(*builtin_commands))
	{
		if (ft_strcmp(cmd_name, builtin_commands[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}

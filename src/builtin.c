/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 13:11:28 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_builtin(t_node *node, t_shell *shell)
{
	int		status;
	char	**argv;

	do_redirect(node->command->redirects);  // リダイレクトの処理
	argv = token_list_to_argv(node->command->args);
	if (ft_strcmp(argv[0], "exit") == 0)
		status = builtin_exit(argv, shell);  // shell構造体を渡す
	else if (ft_strcmp(argv[0], "export") == 0)
		status = builtin_export(argv, shell);
	else if (ft_strcmp(argv[0], "unset") == 0)
		status = builtin_unset(argv, shell);
	else if (ft_strcmp(argv[0], "env") == 0)
		status = builtin_env(argv, shell);
	else if (ft_strcmp(argv[0], "cd") == 0)
		status = builtin_cd(argv, shell);
	else if (ft_strcmp(argv[0], "echo") == 0)
		status = builtin_echo(argv);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		status = builtin_pwd(argv, shell);
	else
		todo("exec_builtin");  // 実装されていないビルトイン処理

	free_argv(argv);  // argvのメモリ解放
	reset_redirect(node->command->redirects);  // リダイレクトのリセット
	return (status);
}

bool	is_builtin(t_node *node)
{
	const char *cmd_name;
	char *builtin_commands[] = {"exit", "export", "unset", "env", "cd", "echo",
		"pwd"};

	unsigned int i;

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
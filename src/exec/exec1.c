/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/28 17:03:40 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		exec_nonbuiltin(t_node *node, t_shell *shell) __attribute__((noreturn));

int	exec_nonbuiltin(t_node *node, t_shell *shell)
{
	char	*path;
	char	**argv;

	do_redirect(node->command->redirects);
	argv = token_list_to_argv(node->command->args);
	path = argv[0];
	if (ft_strchr(path, '/') == NULL)
		path = search_path(path, shell);
	validate_access(path, argv[0]);
	execve(path, argv, get_environ(shell->envmap));
	free_argv(argv);
	reset_redirect(node->command->redirects);
	fatal_error("execve");
}

void	reset_signal(void)
{
	reset_sig(SIGQUIT);
	reset_sig(SIGINT);
}

pid_t	exec_pipeline(t_node *node, t_shell *shell)
{
	pid_t	pid;

	if (node == NULL)
		return (-1);
	prepare_pipe(node);
	pid = fork();
	if (pid < 0)
		fatal_error("fork");
	else if (pid == 0)
	{
		reset_signal();
		prepare_pipe_child(node);
		if (is_builtin(node))
			exit(exec_builtin(node, shell));
		else
			exec_nonbuiltin(node, shell);
	}
	prepare_pipe_parent(node);
	if (node->next)
		return (exec_pipeline(node->next, shell));
	return (pid);
}

int	handle_wait_result(pid_t wait_result, int wstatus, pid_t last_pid)
{
	if (wait_result == last_pid)
	{
		if (WIFSIGNALED(wstatus))
			return (128 + WTERMSIG(wstatus));
		else
			return (WEXITSTATUS(wstatus));
	}
	return (-1);
}

int	wait_pipeline(pid_t last_pid)
{
	pid_t	wait_result;
	int		status;
	int		wstatus;
	int		temp_status;

	status = 0;
	while (1)
	{
		wait_result = wait(&wstatus);
		if (wait_result >= 0)
		{
			temp_status = handle_wait_result(wait_result, wstatus, last_pid);
			if (temp_status >= 0)
				status = temp_status;
		}
		else if (errno == ECHILD)
			break ;
		else if (errno != EINTR)
			fatal_error("wait");
	}
	return (status);
}

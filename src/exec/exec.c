/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 17:47:31 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec(t_node *node, t_shell *shell)
{
	pid_t	last_pid;
	int		status;

	if (open_redir_file(node, shell) < 0)
		return (ERROR_OPEN_REDIR); // リダイレクトエラー時の処理
	if (node->next == NULL && is_builtin(node))
		status = exec_builtin(node, shell); // shell構造体を渡す
	else
	{
		last_pid = exec_pipeline(node, shell); // パイプラインを実行
		status = wait_pipeline(last_pid);      // パイプラインの終了待機
	}
	shell->last_status = status; // 最後のステータスをシェル構造体に保存
	return (status);
}

char	*search_path_mode(const char *filename, int mode, t_shell *shell)
{
	char	path[PATH_MAX];
	char	*value;
	char	*end;
	char	*dup;

	value = xgetenv("PATH", shell);
	while (*value)
	{
		// /bin:/usr/bin
		//     ^
		//     end
		bzero(path, PATH_MAX);
		end = strchr(value, ':');
		if (end)
			strncpy(path, value, end - value);
		else
			strlcpy(path, value, PATH_MAX);
		strlcat(path, "/", PATH_MAX);
		strlcat(path, filename, PATH_MAX);
		if (access(path, mode) == 0)
		{
			dup = ft_strdup(path);
			if (dup == NULL)
				fatal_error("strdup");
			return (dup);
		}
		if (end == NULL)
			return (NULL);
		value = end + 1;
	}
	return (NULL);
}

char	*search_path(const char *filename, t_shell *shell)
{
	char	*path;

	path = search_path_mode(filename, X_OK, shell);
	if (path)
		return (path);
	path = search_path_mode(filename, F_OK, shell);
	return (path);
}

void	validate_access(const char *path, const char *filename)
{
	struct stat	st;

	if (path == NULL)
		err_exit(filename, "command not found", 127);
	if (ft_strcmp(filename, "") == 0)
		err_exit(filename, "command not found", 127);
	if (ft_strcmp(filename, "..") == 0)
		err_exit(filename, "command not found", 127);
	if (access(path, F_OK) < 0)
		err_exit(filename, "command not found", 127);
	if (stat(path, &st) < 0)
		fatal_error("fstat");
	if (S_ISDIR(st.st_mode))
		err_exit(filename, "is a directory", 126);
	if (access(path, X_OK) < 0)
		err_exit(path, "Permission denied", 126);
}

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
	reset_sig(SIGQUIT);  // SIGQUITをデフォルト動作に戻す
	reset_sig(SIGINT);   // SIGINTをデフォルト動作に戻す
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
		// child process
		reset_signal();
		prepare_pipe_child(node);
		if (is_builtin(node))
			exit(exec_builtin(node, shell));
		else
			exec_nonbuiltin(node, shell);
	}
	// parent process
	prepare_pipe_parent(node);
	if (node->next)
		return (exec_pipeline(node->next, shell));
	return (pid);
}

int	wait_pipeline(pid_t last_pid)
{
	pid_t wait_result;
	int status;
	int wstatus;

	while (1)
	{
		wait_result = wait(&wstatus);
		if (wait_result == last_pid)
		{
			if (WIFSIGNALED(wstatus))
				status = 128 + WTERMSIG(wstatus);
			else
				status = WEXITSTATUS(wstatus);
		}
		else if (wait_result < 0)
		{
			if (errno == ECHILD)
				break ;
			else if (errno == EINTR)
				continue ;
			else
				fatal_error("wait");
		}
	}
	return (status);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 20:16:58 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec(t_node *node, t_shell *shell)
{
	pid_t	last_pid;
	int		status;

	if (open_redir_file(node, shell) < 0)
		return (ERROR_OPEN_REDIR);
	if (node->next == NULL && is_builtin(node))
		status = exec_builtin(node, shell);
	else
	{
		last_pid = exec_pipeline(node, shell);
		status = wait_pipeline(last_pid);
	}
	shell->last_status = status;
	return (status);
}

char	*build_path(const char *value, const char *filename)
{
	char	path[PATH_MAX];
	char	*end;

	ft_memset(path, 0, PATH_MAX);
	end = ft_strchr(value, ':');
	if (end)
		ft_strncpy(path, value, end - value);
	else
		ft_strlcpy(path, value, PATH_MAX);
	ft_strlcat(path, "/", PATH_MAX);
	ft_strlcat(path, filename, PATH_MAX);
	return (ft_strdup(path));
}

char	*search_path_mode(const char *filename, int mode, t_shell *shell)
{
	char	*value;
	char	*path;
	char	*dup;

	value = xgetenv("PATH", shell);
	while (*value)
	{
		path = build_path(value, filename);
		if (access(path, mode) == 0)
		{
			dup = ft_strdup(path);
			free(path);
			if (dup == NULL)
				fatal_error("strdup");
			return (dup);
		}
		free(path);
		value = ft_strchr(value, ':');
		if (!value)
			return (NULL);
		value++;
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

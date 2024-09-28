/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 21:30:02 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_directory(char *path)
{
	if (chdir(path) < 0)
	{
		builtin_error("cd", NULL, "error de iinjanaisuka by sagemura");
		return (1);
	}
	return (0);
}

int	cd_to_home(char *home, char *path)
{
	if (home == NULL)
	{
		builtin_error("cd", NULL, "HOME not set");
		return (1);
	}
	ft_strlcpy(path, home, PATH_MAX);
	return (0);
}

int	update_pwd(t_shell *shell, char *oldpwd, char *path)
{
	char	*newpwd;

	newpwd = resolve_pwd(oldpwd, path);
	map_set(shell->envmap, "PWD", newpwd);
	free(newpwd);
	return (0);
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	path[PATH_MAX];
	char	*oldpwd;
	char	*home;

	oldpwd = map_get(shell->envmap, "PWD");
	map_set(shell->envmap, "OLDPWD", oldpwd);
	if (argv[1] == NULL)
	{
		home = map_get(shell->envmap, "HOME");
		if (cd_to_home(home, path))
			return (1);
	}
	else
		ft_strlcpy(path, argv[1], PATH_MAX);
	if (change_directory(path))
		return (1);
	return (update_pwd(shell, oldpwd, path));
}

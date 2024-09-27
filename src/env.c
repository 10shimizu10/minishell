/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 17:34:30 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*xgetenv(const char *name, t_shell *shell)
{
	return (map_get(shell->envmap, name));
}

char	**allocate_environ(size_t size)
{
	char	**environ;
	size_t	i;

	environ = malloc(size * sizeof(char *));
	if (!environ)
		return (NULL);
	i = 0;
	while (i < size)
	{
		environ[i] = NULL;
		i++;
	}
	return (environ);
}

char	**get_environ(t_map *map)
{
	size_t	i;
	t_item	*item;
	char	**environ;
	size_t	size;

	size = map_len(map, false) + 1;
	environ = allocate_environ(size);
	if (!environ)
		return (NULL);
	i = 0;
	item = map->item_head.next;
	while (item)
	{
		if (item->value)
		{
			environ[i] = item_get_string(item);
			i++;
		}
		item = item->next;
	}
	return (environ);
}

static void	envmap_init(t_map *map, char **ep)
{
	char	cwd[PATH_MAX];

	while (*ep)
	{
		map_put(map, *ep, false);
		ep++;
	}
	if (map_get(map, "SHLVL") == NULL)
		map_set(map, "SHLVL", "1");
	if (map_get(map, "PWD") == NULL)
	{
		getcwd(cwd, PATH_MAX);
		map_set(map, "PWD", cwd);
	}
	if (map_get(map, "OLDPWD") == NULL)
		map_set(map, "OLDPWD", NULL);
}

void	initenv(t_shell *shell)
{
	extern char	**environ;

	shell->envmap = map_new();
	envmap_init(shell->envmap, environ);
}

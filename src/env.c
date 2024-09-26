/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 00:38:39 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_map		*envmap;

static void	envmap_init(t_map *map, char **ep);

char	*xgetenv(const char *name)
{
	return (map_get(envmap, name));
}

void	initenv(void)
{
	extern char	**environ;

	envmap = map_new();
	envmap_init(envmap, environ);
}

char	**get_environ(t_map *map)
{
    size_t	i;
    size_t	size;
    t_item	*item;
    char	**environ;

    size = map_len(map, false) + 1;
    environ = malloc(size * sizeof(char *));  // mallocを使用してメモリ確保
    if (!environ) {
        return NULL; // メモリ確保失敗時はNULLを返す
    }

    // 確保したメモリをゼロクリア (while を使う)
    i = 0;
    while (i < size)
    {
        environ[i] = NULL;
        i++;
    }

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
    environ[i] = NULL;
    return (environ);
}

static void	envmap_init(t_map *map, char **ep)
{
	char cwd[PATH_MAX];
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
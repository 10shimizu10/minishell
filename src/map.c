/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 00:57:15 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_identifier(const char *s)
{
	if (!is_alpha_under(*s))
		return (false);
	s++;
	while (*s)
	{
		if (!is_alpha_num_under(*s))
			return (false);
		s++;
	}
	return (true);
}

t_item *item_new(char *name, char *value)
{
    t_item *item;

    // calloc の代わりに malloc を使ってメモリを確保
    item = malloc(sizeof(*item));
    if (item == NULL)
        fatal_error("malloc");

    // メモリをゼロクリアする (必要なら手動で行う)
    item->name = NULL;
    item->value = NULL;

    // name と value をセット
    item->name = name;
    item->value = value;

    return item;
}

char	*item_get_string(t_item *item)
{
	size_t	strsize;
	char	*string;

	strsize = ft_strlen(item->name) + 2;
	if (item->value)
		strsize += ft_strlen(item->value);
	string = malloc(strsize);
	if (string == NULL)
		fatal_error("malloc");
	ft_strlcpy(string, item->name, strsize);
	if (item->value)
	{
		ft_strlcat(string, "=", strsize);
		ft_strlcat(string, item->value, strsize);
	}
	return (string);
}

t_map *map_new(void)
{
    t_map *map;

    // calloc の代わりに malloc を使用してメモリを確保
    map = malloc(sizeof(*map));
    if (map == NULL)
        fatal_error("malloc");

    // メモリをゼロクリアする
    // これで calloc と同様に、すべてのメンバがゼロ初期化されます
    // (メンバにポインタや構造体がある場合には、NULLや0で初期化する)
    *map = (t_map){0};

    return map;
}

char	*map_get(t_map *map, const char *name)
{
	t_item	*cur;

	if (name == NULL)
		return (NULL);
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

int	map_unset(t_map *map, const char *name)
{
	t_item	*cur;
	t_item	*prev;

	if (name == NULL || !is_identifier(name))
		return (-1);
	prev = &map->item_head;
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
		{
			prev->next = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			return (0);
		}
		prev = prev->next;
		cur = cur->next;
	}
	return (0);
}

// name, value
int	map_set(t_map *map, const char *name, const char *value)
{
	t_item	*cur;

	if (name == NULL || !is_identifier(name))
		return (-1);
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
			break ;
		cur = cur->next;
	}
	// found
	if (cur)
	{
		free(cur->value);
		if (value == NULL)
			cur->value = NULL;
		else
		{
			cur->value = ft_strdup(value);
			if (cur->value == NULL)
				fatal_error("map_set strdup");
		}
	}
	// not found
	else
	{
		if (value == NULL)
		{
			cur = item_new(strdup(name), NULL);
			if (cur->name == NULL)
				fatal_error("strdup");
		}
		else
		{
			cur = item_new(strdup(name), ft_strdup(value));
			if (cur->name == NULL || cur->value == NULL)
				fatal_error("strdup");
		}
		cur->next = map->item_head.next;
		map->item_head.next = cur;
	}
	return (0);
}

int	map_put(t_map *map, const char *string, bool allow_empty_value)
{
	int		result;
	char	*name_end;
	char	*name;
	char	*value;

	name_end = ft_strchr(string, '=');
	// value is null
	if (name_end == NULL)
	{
		if (!allow_empty_value)
			return (-1);
		name = ft_strdup(string);
		value = NULL;
		if (name == NULL)
			fatal_error("strdup");
	}
	// value is non null
	else
	{
		name = ft_strndup(string, name_end - string);
		value = ft_strdup(name_end + 1);
		if (name == NULL || value == NULL)
			fatal_error("strdup");
	}
	result = map_set(map, name, value);
	free(name);
	free(value);
	return (result);
}

size_t	map_len(t_map *map, bool count_null_value)
{
	size_t len;
	t_item *item;

	len = 0;
	item = map->item_head.next;
	while (item)
	{
		if (item->value || count_null_value)
			len++;
		item = item->next;
	}
	return (len);
}
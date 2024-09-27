/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 18:27:48 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_name_value(const char *string, char **name, char **value,
		bool allow_empty_value)
{
	char	*name_end;

	name_end = ft_strchr(string, '=');
	if (name_end == NULL)
	{
		if (!allow_empty_value)
			fatal_error("invalid string, no '=' found");
		*name = ft_strdup(string);
		*value = NULL;
	}
	else
	{
		*name = ft_strndup(string, name_end - string);
		*value = ft_strdup(name_end + 1);
	}
	if (*name == NULL || (name_end != NULL && *value == NULL))
		fatal_error("strdup");
}

int	map_put(t_map *map, const char *string, bool allow_empty_value)
{
	int		result;
	char	*name;
	char	*value;

	split_name_value(string, &name, &value, allow_empty_value);
	result = map_set(map, name, value);
	free(name);
	free(value);
	return (result);
}

size_t	map_len(t_map *map, bool count_null_value)
{
	size_t	len;
	t_item	*item;

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

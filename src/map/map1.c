/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 18:27:57 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_item	*find_item(t_item *head, const char *name)
{
	t_item	*cur;

	cur = head->next;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

void	update_item(t_item *item, const char *value)
{
	free(item->value);
	if (value == NULL)
		item->value = NULL;
	else
	{
		item->value = ft_strdup(value);
		if (item->value == NULL)
			fatal_error("map_set strdup");
	}
}

t_item	*create_new_item(const char *name, const char *value)
{
	t_item	*new_item;

	if (value == NULL)
		new_item = item_new(ft_strdup(name), NULL);
	else
		new_item = item_new(ft_strdup(name), ft_strdup(value));
	if (new_item->name == NULL || (value != NULL && new_item->value == NULL))
		fatal_error("strdup");
	return (new_item);
}

int	map_set(t_map *map, const char *name, const char *value)
{
	t_item	*cur;

	if (name == NULL || !is_identifier(name))
		return (-1);
	cur = find_item(&map->item_head, name);
	if (cur)
		update_item(cur, value);
	else
	{
		cur = create_new_item(name, value);
		cur->next = map->item_head.next;
		map->item_head.next = cur;
	}
	return (0);
}

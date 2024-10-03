/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/28 16:29:48 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += ft_strlen(*s);
	new = malloc(size);
	if (new == NULL)
		fatal_error("malloc");
	if (*s)
		ft_strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

void	remove_quote_pair(char **dst, char **rest, char *p, char quote_char)
{
	if (*p == quote_char)
	{
		p++;
		while (*p != quote_char)
		{
			if (*p == '\0')
				assert_error("Unclosed quote");
			append_char(dst, *p++);
		}
		p++;
		*rest = p;
	}
	else
		assert_error("Expected quote");
}

void	remove_quote(t_token *token)
{
	char	*new_word;
	char	*p;

	if (token == NULL || token->kind != TOKEN_WORD || token->word == NULL)
		return ;
	p = token->word;
	new_word = malloc(sizeof(char));
	if (new_word == NULL)
		fatal_error("malloc");
	ft_memset(new_word, 0, sizeof(char));
	while (*p && !is_metacharacter(*p))
	{
		if (*p == SINGLE_QUOTE_CHAR || *p == DOUBLE_QUOTE_CHAR)
			remove_quote_pair(&new_word, &p, p, *p);
		else
			append_char(&new_word, *p++);
	}
	free(token->word);
	token->word = new_word;
	remove_quote(token->next);
}

void	expand_quote_removal(t_node *node)
{
	if (node == NULL)
		return ;
	remove_quote(node->args);
	remove_quote(node->filename);
	remove_quote(node->delimiter);
	expand_quote_removal(node->redirects);
	expand_quote_removal(node->command);
	expand_quote_removal(node->next);
}

void	append_num(char **dst, unsigned int num)
{
	if (num == 0)
	{
		append_char(dst, '0');
		return ;
	}
	if (num / 10 != 0)
		append_num(dst, num / 10);
	append_char(dst, '0' + (num % 10));
}

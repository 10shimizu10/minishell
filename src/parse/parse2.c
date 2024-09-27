/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 20:44:10 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	equal_op(t_token *token, char *op)
{
	if (token->kind != TOKEN_OP)
		return (false);
	return (ft_strcmp(token->word, op) == 0);
}

t_node	*new_node(t_node_type type)
{
	t_node	*node;

	node = malloc(sizeof(*node));
	if (node == NULL)
		fatal_error("malloc");
	ft_memset(node, 0, sizeof(*node));
	node->kind = type;
	return (node);
}

t_token	*token_dup(t_token *token)
{
	char	*word;

	word = ft_strdup(token->word);
	if (word == NULL)
		fatal_error("strdup");
	return (new_token(word, token->kind));
}

void	append_token(t_token **token, t_token *elm)
{
	if (*token == NULL)
	{
		*token = elm;
		return ;
	}
	append_token(&(*token)->next, elm);
}

void	append_node(t_node **node, t_node *elm)
{
	if (*node == NULL)
	{
		*node = elm;
		return ;
	}
	append_node(&(*node)->next, elm);
}
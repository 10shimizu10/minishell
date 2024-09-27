/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 20:43:51 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*redirect_out(t_token **rest, t_token *token)
{
	t_node	*node;

	node = new_node(ND_REDIR_OUT);
	node->filename = token_dup(token->next);
	node->targetfd = STDOUT_FILENO;
	*rest = token->next->next;
	return (node);
}

t_node	*redirect_in(t_token **rest, t_token *token)
{
	t_node	*node;

	node = new_node(ND_REDIR_IN);
	node->filename = token_dup(token->next);
	node->targetfd = STDIN_FILENO;
	*rest = token->next->next;
	return (node);
}

t_node	*redirect_append(t_token **rest, t_token *token)
{
	t_node	*node;

	node = new_node(ND_REDIR_APPEND);
	node->filename = token_dup(token->next);
	node->targetfd = STDOUT_FILENO;
	*rest = token->next->next;
	return (node);
}

t_node	*redirect_heredoc(t_token **rest, t_token *token)
{
	t_node	*node;

	node = new_node(ND_REDIR_HEREDOC);
	node->delimiter = token_dup(token->next);
	if (ft_strchr(node->delimiter->word, SINGLE_QUOTE_CHAR) == NULL
		&& ft_strchr(node->delimiter->word, DOUBLE_QUOTE_CHAR) == NULL)
		node->is_delim_unquoted = true;
	node->targetfd = STDIN_FILENO;
	*rest = token->next->next;
	return (node);
}

void	append_command_element(t_node *command, t_token **rest, t_token *token)
{
	if (token->kind == TOKEN_WORD)
	{
		append_token(&command->args, token_dup(token));
		token = token->next;
	}
	else if (equal_op(token, ">") && token->next->kind == TOKEN_WORD)
		append_node(&command->redirects, redirect_out(&token, token));
	else if (equal_op(token, "<") && token->next->kind == TOKEN_WORD)
		append_node(&command->redirects, redirect_in(&token, token));
	else if (equal_op(token, ">>") && token->next->kind == TOKEN_WORD)
		append_node(&command->redirects, redirect_append(&token, token));
	else if (equal_op(token, "<<") && token->next->kind == TOKEN_WORD)
		append_node(&command->redirects, redirect_heredoc(&token, token));
	else
		todo("append_command_element");
	*rest = token;
}
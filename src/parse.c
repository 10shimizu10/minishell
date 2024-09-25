/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/25 21:18:05 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	equal_op(t_token *tok, char *op);
void	append_node(t_node **node, t_node *elm);
t_node	*pipeline(t_token **rest, t_token *token);
t_node	*simple_command(t_token **rest, t_token *token);

t_node	*parse(t_token *token)
{
	return (pipeline(&token, token));
}

t_node	*pipeline(t_token **rest, t_token *token)
{
	t_node	*node;

	node = new_node(ND_PIPELINE);
	node->inpipe[0] = STDIN_FILENO;
	node->inpipe[1] = -1;
	node->outpipe[0] = -1;
	node->outpipe[1] = STDOUT_FILENO;
	node->command = simple_command(&token, token);
	if (equal_op(token, "|"))
		node->next = pipeline(&token, token->next);
	*rest = token;
	return (node);
}

bool	is_control_operator(t_token *tok)
{
	size_t	i;

	static char *const operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|",
		"\n"};
	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(tok->word, operators[i]))
			return (true);
		i++;
	}
	return (false);
}

t_node	*simple_command(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_SIMPLE_CMD);
	append_command_element(node, &tok, tok);
	while (tok && !at_eof(tok) && !is_control_operator(tok))
		append_command_element(node, &tok, tok);
	*rest = tok;
	return (node);
}

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
	if (strchr(node->delimiter->word, SINGLE_QUOTE_CHAR) == NULL
		&& strchr(node->delimiter->word, DOUBLE_QUOTE_CHAR) == NULL)
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

bool	at_eof(t_token *token)
{
	return (token->kind == TOKEN_EOF);
}

bool	equal_op(t_token *token, char *op)
{
	if (token->kind != TOKEN_OP)
		return (false);
	return (strcmp(token->word, op) == 0);
}

t_node	*new_node(t_node_type type)
{
	t_node	*node;

	node = malloc(sizeof(*node));
	if (node == NULL)
		fatal_error("malloc");
	memset(node, 0, sizeof(*node));
	node->kind = type;
	return (node);
}

t_token	*token_dup(t_token *token)
{
	char	*word;

	word = strdup(token->word);
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
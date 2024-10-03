/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/28 17:52:32 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	initialize_operators(char **operators)
{
	operators[0] = "||";
	operators[1] = "&";
	operators[2] = "&&";
	operators[3] = ";";
	operators[4] = ";;";
	operators[5] = "(";
	operators[6] = ")";
	operators[7] = "|";
	operators[8] = "\n";
}

bool	is_control_operator(t_token *tok)
{
	size_t	i;
	char	*operators[CONTROL_OPERATOR_COUNT];

	initialize_operators(operators);
	i = 0;
	while (i < CONTROL_OPERATOR_COUNT)
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

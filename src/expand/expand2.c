/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/28 14:30:03 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_word_segment(char **new_word, char **p, t_shell *shell)
{
	if (**p == SINGLE_QUOTE_CHAR)
		append_single_quote(new_word, p, *p);
	else if (**p == DOUBLE_QUOTE_CHAR)
		append_double_quote(new_word, p, *p, shell);
	else if (is_variable(*p))
		expand_variable_str(new_word, p, *p, shell);
	else if (is_special_parameter(*p))
		expand_special_parameter_str(new_word, p, *p, shell);
	else
		append_char(new_word, *(*p)++);
}

void	expand_variable_token(t_token *token, t_shell *shell)
{
	char	*new_word;
	char	*p;

	if (token == NULL || token->kind != TOKEN_WORD || token->word == NULL)
		return ;
	p = token->word;
	new_word = malloc(1 * sizeof(char));
	if (new_word == NULL)
		fatal_error("malloc");
	ft_memset(new_word, 0, 1 * sizeof(char));
	while (*p && !is_metacharacter(*p))
		process_word_segment(&new_word, &p, shell);
	free(token->word);
	token->word = new_word;
	expand_variable_token(token->next, shell);
}

void	expand_variable(t_node *node, t_shell *shell)
{
	if (node == NULL)
		return ;
	expand_variable_token(node->args, shell);
	expand_variable_token(node->filename, shell);
	expand_variable(node->redirects, shell);
	expand_variable(node->command, shell);
	expand_variable(node->next, shell);
}

void	expand(t_node *node, t_shell *shell)
{
	expand_variable(node, shell);
	expand_quote_removal(node);
}

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	char	*new_word;
	char	*p;

	p = line;
	new_word = malloc(1 * sizeof(char));
	if (new_word == NULL)
		fatal_error("malloc");
	ft_memset(new_word, 0, 1 * sizeof(char));
	while (*p)
	{
		if (is_variable(p))
			expand_variable_str(&new_word, &p, p, shell);
		else if (is_special_parameter(p))
			expand_special_parameter_str(&new_word, &p, p, shell);
		else
			append_char(&new_word, *p++);
	}
	free(line);
	return (new_word);
}

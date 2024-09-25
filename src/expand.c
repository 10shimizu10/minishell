/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/25 21:09:52 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += strlen(*s);
	new = malloc(size);
	if (new == NULL)
		fatal_error("malloc");
	if (*s)
		strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

void	remove_single_quote(char **dst, char **rest, char *p)
{
	if (*p == SINGLE_QUOTE_CHAR)
	{
		// skip quote
		p++;
		while (*p != SINGLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed single quote");
			append_char(dst, *p++);
		}
		// skip quote
		p++;
		*rest = p;
	}
	else
		assert_error("Expected single quote");
}

void	remove_double_quote(char **dst, char **rest, char *p)
{
	if (*p == DOUBLE_QUOTE_CHAR)
	{
		// skip quote
		p++;
		while (*p != DOUBLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed double quote");
			append_char(dst, *p++);
		}
		// skip quote
		p++;
		*rest = p;
	}
	else
		assert_error("Expected double quote");
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
	memset(new_word, 0, sizeof(char));
	while (*p && !is_metacharacter(*p))
	{
		if (*p == SINGLE_QUOTE_CHAR)
			remove_single_quote(&new_word, &p, p);
		else if (*p == DOUBLE_QUOTE_CHAR)
			remove_double_quote(&new_word, &p, p);
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

bool	is_alpha(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

bool	is_alpha_under(char c)
{
	return (is_alpha(c) || c == '_');
}

bool	is_alpha_num_under(char c)
{
	return (is_alpha_under(c) || is_digit(c));
}

bool	is_variable(char *s)
{
	return (s[0] == '$' && is_alpha_under(s[1]));
}

bool	is_special_parameter(char *s)
{
	return (s[0] == '$' && s[1] == '?');
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

void	expand_special_parameter_str(char **dst, char **rest, char *p)
{
	if (!is_special_parameter(p))
		assert_error("Expected special parameter");
	p += 2;
	append_num(dst, last_status);
	*rest = p;
}

void	expand_variable_str(char **dst, char **rest, char *p)
{
	char	*name;
	char	*value;

	name = calloc(1, sizeof(char));
	if (name == NULL)
		fatal_error("calloc");
	if (*p != '$')
		assert_error("Expected dollar sign");
	p++;
	if (!is_alpha_under(*p))
		assert_error("Variable must starts with alphabetic character or underscore.");
	append_char(&name, *p++);
	while (is_alpha_num_under(*p))
		append_char(&name, *p++);
	value = xgetenv(name);
	free(name);
	if (value)
		while (*value)
			append_char(dst, *value++);
	*rest = p;
}

void	append_single_quote(char **dst, char **rest, char *p)
{
	if (*p == SINGLE_QUOTE_CHAR)
	{
		// skip quote
		append_char(dst, *p++);
		while (*p != SINGLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed single quote");
			append_char(dst, *p++);
		}
		// skip quote
		append_char(dst, *p++);
		*rest = p;
	}
	else
		assert_error("Expected single quote");
}

void	append_double_quote(char **dst, char **rest, char *p)
{
	if (*p == DOUBLE_QUOTE_CHAR)
	{
		// skip quote
		append_char(dst, *p++);
		while (*p != DOUBLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed double quote");
			else if (is_variable(p))
				expand_variable_str(dst, &p, p);
			else if (is_special_parameter(p))
				expand_special_parameter_str(dst, &p, p);
			else
				append_char(dst, *p++);
		}
		// skip quote
		append_char(dst, *p++);
		*rest = p;
	}
	else
		assert_error("Expected double quote");
}

void	expand_variable_token(t_token *token)
{
	char	*new_word;
	char	*p;

	if (token == NULL || token->kind != TOKEN_WORD || token->word == NULL)
		return ;
	p = token->word;
	new_word = malloc(1 * sizeof(char));
	if (new_word == NULL)
		fatal_error("malloc");
	memset(new_word, 0, 1 * sizeof(char));
	if (new_word == NULL)
		fatal_error("calloc");
	while (*p && !is_metacharacter(*p))
	{
		if (*p == SINGLE_QUOTE_CHAR)
			append_single_quote(&new_word, &p, p);
		else if (*p == DOUBLE_QUOTE_CHAR)
			append_double_quote(&new_word, &p, p);
		else if (is_variable(p))
			expand_variable_str(&new_word, &p, p);
		else if (is_special_parameter(p))
			expand_special_parameter_str(&new_word, &p, p);
		else
			append_char(&new_word, *p++);
	}
	free(token->word);
	token->word = new_word;
	expand_variable_token(token->next);
}

void	expand_variable(t_node *node)
{
	if (node == NULL)
		return ;
	expand_variable_token(node->args);
	expand_variable_token(node->filename);
	// do not expand heredoc delimiter
	expand_variable(node->redirects);
	expand_variable(node->command);
	expand_variable(node->next);
}

void	expand(t_node *node)
{
	expand_variable(node);
	expand_quote_removal(node);
}

char	*expand_heredoc_line(char *line)
{
	char *new_word;
	char *p;

	p = line;
	new_word = malloc(1 * sizeof(char));
	if (new_word == NULL)
		fatal_error("malloc");

	memset(new_word, 0, 1 * sizeof(char));

	while (*p)
	{
		if (is_variable(p))
			expand_variable_str(&new_word, &p, p);
		else if (is_special_parameter(p))
			expand_special_parameter_str(&new_word, &p, p);
		else
			append_char(&new_word, *p++);
	}
	free(line);
	return (new_word);
}
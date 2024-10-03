/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/28 17:04:56 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_special_parameter_str(char **dst, char **rest, char *p,
		t_shell *shell)
{
	if (!is_special_parameter(p))
		assert_error("Expected special parameter");
	p += 2;
	append_num(dst, shell->last_status);
	*rest = p;
}

static char	*get_variable_name(char **p)
{
	char	*name;

	name = malloc(1 * sizeof(char));
	if (name == NULL)
		fatal_error("malloc");
	name[0] = '\0';
	if (**p != '$')
		assert_error("Expected dollar sign");
	(*p)++;
	if (!is_alpha_under(**p))
		assert_error("Variable must start with an alphabet or underscore.");
	append_char(&name, *(*p)++);
	while (is_alpha_num_under(**p))
		append_char(&name, *(*p)++);
	return (name);
}

void	expand_variable_str(char **dst, char **rest, char *p, t_shell *shell)
{
	char	*name;
	char	*value;

	name = get_variable_name(&p);
	value = xgetenv(name, shell);
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
		append_char(dst, *p++);
		while (*p != SINGLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed single quote");
			append_char(dst, *p++);
		}
		append_char(dst, *p++);
		*rest = p;
	}
	else
		assert_error("Expected single quote");
}

void	append_double_quote(char **dst, char **rest, char *p, t_shell *shell)
{
	if (*p == DOUBLE_QUOTE_CHAR)
	{
		append_char(dst, *p++);
		while (*p != DOUBLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed double quote");
			else if (is_variable(p))
				expand_variable_str(dst, &p, p, shell);
			else if (is_special_parameter(p))
				expand_special_parameter_str(dst, &p, p, shell);
			else
				append_char(dst, *p++);
		}
		append_char(dst, *p++);
		*rest = p;
	}
	else
		assert_error("Expected double quote");
}

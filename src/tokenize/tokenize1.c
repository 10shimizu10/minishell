/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/28 17:39:33 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**allocate_operators(void)
{
	char	**operators;

	operators = malloc(OPERATOR_COUNT * sizeof(char *));
	if (operators == NULL)
		fatal_error("malloc");
	return (operators);
}

void	assign_operators(char **operators)
{
	operators[0] = ">>";
	operators[1] = "<<";
	operators[2] = "||";
	operators[3] = "&&";
	operators[4] = ";;";
	operators[5] = "<";
	operators[6] = ">";
	operators[7] = "&";
	operators[8] = ";";
	operators[9] = "(";
	operators[10] = ")";
	operators[11] = "|";
	operators[12] = "\n";
}

static char	**initialize_operators(void)
{
	char	**operators;

	operators = allocate_operators();
	assign_operators(operators);
	return (operators);
}

t_token	*find_operator(char **rest, char *line, char **operators)
{
	size_t	i;
	char	*op;

	i = 0;
	while (i < OPERATOR_COUNT)
	{
		if (startswith(line, operators[i]))
		{
			op = ft_strdup(operators[i]);
			if (op == NULL)
				fatal_error("strdup");
			*rest = line + ft_strlen(op);
			return (new_token(op, TOKEN_OP));
		}
		i++;
	}
	return (NULL);
}

t_token	*operator(char **rest, char *line)
{
	t_token	*token;
	char	**operators;

	operators = initialize_operators();
	token = find_operator(rest, line, operators);
	free(operators);
	if (token == NULL)
		assert_error("Unexpected operator");
	return (token);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/25 21:18:54 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

int		last_status;

void	interpret(char *line, int *stat_loc)
{
	t_token	*token;
	t_node	*node;

	token = tokenize(line);
	if (at_eof(token))
		;
	else if (syntax_error)
		*stat_loc = ERROR_TOKENIZE;
	else
	{
		node = parse(token);
		if (syntax_error)
			*stat_loc = ERROR_PARSE;
		if (!syntax_error)
		{
			expand(node);
			*stat_loc = exec(node);
		}
		free_node(node);
	}
	free_token(token);
}

int	main(void)
{
	char *line;

	rl_outstream = stderr;
	initenv();
	setup_signal();
	last_status = 0;

	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;

		if (*line)
			add_history(line);
		interpret(line, &last_status);
		free(line);
	}
	// write_history("history.txt");
	exit(last_status);
}
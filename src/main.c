/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 14:27:31 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	interpret(char *line, t_shell *shell)
{
	t_token	*token;
	t_node	*node;

	token = tokenize(line, shell);
	if (at_eof(token))
		;
	else if (shell->syntax_error)
		shell->last_status = ERROR_TOKENIZE;
	else
	{
		node = parse(token);
		if (shell->syntax_error)
			shell->last_status = ERROR_PARSE;
		if (!shell->syntax_error)
		{
			expand(node, shell);
			shell->last_status = exec(node, shell);
		}
		free_node(node);
	}
	free_token(token);
}

int	main(void)
{
	char	*line;
	t_shell	shell;

	shell.last_status = 0;
	shell.syntax_error = false;
	shell.readline_interrupted = false;
	rl_outstream = stderr;
	initenv(&shell);
	setup_signal();
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		interpret(line, &shell);
		free(line);
	}
	// write_history("history.txt");
	exit(shell.last_status);
}

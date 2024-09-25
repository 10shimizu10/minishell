/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/25 21:19:06 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define ERROR_PREFIX "minishell: "

bool		syntax_error = false;

static void	perror_prefix(void)
{
	write(STDERR_FILENO, ERROR_PREFIX, sizeof(ERROR_PREFIX) - 1);
}

void	fatal_error(const char *msg)
{
	const char	*prefix = "Fatal Error: ";

	perror_prefix();
	write(STDERR_FILENO, prefix, strlen(prefix));
	write(STDERR_FILENO, msg, strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(1);
}

void	assert_error(const char *msg)
{
	const char	*prefix = "Assert Error: ";

	perror_prefix();
	write(STDERR_FILENO, prefix, strlen(prefix));
	write(STDERR_FILENO, msg, strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(255);
}

void	err_exit(const char *location, const char *msg, int status)
{
	perror_prefix();
	write(STDERR_FILENO, location, strlen(location));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, msg, strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(status);
}

void	todo(const char *msg)
{
	const char	*prefix = "TODO: ";

	perror_prefix();
	write(STDERR_FILENO, prefix, strlen(prefix));
	write(STDERR_FILENO, msg, strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(255);
}

void	tokenize_error(const char *location, char **rest, char *line)
{
	const char	*error_msg_part1 = "syntax error near unexpected character `";
	const char	*error_msg_part2 = "' in ";
	const char	*error_msg_part3 = "\n";

	syntax_error = true;
	perror_prefix();
	write(STDERR_FILENO, error_msg_part1, sizeof(error_msg_part1) - 1);
	write(STDERR_FILENO, line, 1);
	write(STDERR_FILENO, error_msg_part2, sizeof(error_msg_part2) - 1);
	write(STDERR_FILENO, location, strlen(location));
	write(STDERR_FILENO, error_msg_part3, sizeof(error_msg_part3) - 1);
	while (*line)
		line++;
	*rest = line;
}

void	parse_error(const char *location, t_token **rest, t_token *tok)
{
	const char	*error_msg_part1 = "syntax error near unexpected token `";
	const char	*error_msg_part2 = "' in ";
	const char	*error_msg_part3 = "\n";

	syntax_error = true;
	perror_prefix();
	write(STDERR_FILENO, error_msg_part1, sizeof(error_msg_part1) - 1);
	write(STDERR_FILENO, tok->word, strlen(tok->word));
	write(STDERR_FILENO, error_msg_part2, sizeof(error_msg_part2) - 1);
	write(STDERR_FILENO, location, strlen(location));
	write(STDERR_FILENO, error_msg_part3, sizeof(error_msg_part3) - 1);
	while (tok && !at_eof(tok))
		tok = tok->next;
	*rest = tok;
}

void	xperror(const char *location)
{
	perror_prefix();
	perror(location);
}

void	builtin_error(const char *func, const char *name, const char *err)
{
	perror_prefix();
	dprintf(STDERR_FILENO, "%s: ", func);
	if (name)
		dprintf(STDERR_FILENO, "`%s': ", name);
	dprintf(STDERR_FILENO, "%s\n", err);
}
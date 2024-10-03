/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/28 16:29:40 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenize_error(const char *location, char **rest, char *line,
		t_shell *shell)
{
	const char	*error_msg_part1 = "syntax error near unexpected character `";
	const char	*error_msg_part2 = "' in ";
	const char	*error_msg_part3 = "\n";

	shell->syntax_error = true;
	perror_prefix();
	write(STDERR_FILENO, error_msg_part1, sizeof(error_msg_part1) - 1);
	write(STDERR_FILENO, line, 1);
	write(STDERR_FILENO, error_msg_part2, sizeof(error_msg_part2) - 1);
	write(STDERR_FILENO, location, ft_strlen(location));
	write(STDERR_FILENO, error_msg_part3, sizeof(error_msg_part3) - 1);
	while (*line)
		line++;
	*rest = line;
}

void	parse_error(const char *location, t_token **rest, t_token *tok,
		t_shell *shell)
{
	const char	*error_msg_part1 = "syntax error near unexpected token `";
	const char	*error_msg_part2 = "' in ";
	const char	*error_msg_part3 = "\n";

	shell->syntax_error = true;
	perror_prefix();
	write(STDERR_FILENO, error_msg_part1, sizeof(error_msg_part1) - 1);
	write(STDERR_FILENO, tok->word, ft_strlen(tok->word));
	write(STDERR_FILENO, error_msg_part2, sizeof(error_msg_part2) - 1);
	write(STDERR_FILENO, location, ft_strlen(location));
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
	write(STDERR_FILENO, func, ft_strlen(func));
	write(STDERR_FILENO, ": ", 2);
	if (name)
	{
		write(STDERR_FILENO, "`", 1);
		write(STDERR_FILENO, name, ft_strlen(name));
		write(STDERR_FILENO, "': ", 3);
	}
	write(STDERR_FILENO, err, ft_strlen(err));
	write(STDERR_FILENO, "\n", 1);
}

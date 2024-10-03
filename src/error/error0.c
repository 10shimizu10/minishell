/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/28 16:29:32 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_prefix(void)
{
	write(STDERR_FILENO, ERROR_PREFIX, sizeof(ERROR_PREFIX) - 1);
}

void	fatal_error(const char *msg)
{
	const char	*prefix = "Fatal Error: ";

	perror_prefix();
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(1);
}

void	assert_error(const char *msg)
{
	const char	*prefix = "Assert Error: ";

	perror_prefix();
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(255);
}

void	err_exit(const char *location, const char *msg, int status)
{
	perror_prefix();
	write(STDERR_FILENO, location, ft_strlen(location));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(status);
}

void	todo(const char *msg)
{
	const char	*prefix = "TODO: ";

	perror_prefix();
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(255);
}

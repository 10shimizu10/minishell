/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize0.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/28 17:33:21 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *word, t_token_type kind)
{
	t_token	*token;

	token = malloc(sizeof(*token));
	if (token == NULL)
		fatal_error("malloc");
	else
		ft_memset(token, 0, sizeof(*token));
	token->word = word;
	token->kind = kind;
	return (token);
}

bool	consume_blank(char **rest, char *line)
{
	if (is_blank(*line))
	{
		while (*line && is_blank(*line))
			line++;
		*rest = line;
		return (true);
	}
	*rest = line;
	return (false);
}

bool	startswith(const char *s, const char *keyword)
{
	return (ft_memcmp(s, keyword, ft_strlen(keyword)) == 0);
}

char	*process_quote(char *line, char quote_char, t_shell *shell)
{
	line++;
	while (*line && *line != quote_char)
		line++;
	if (*line == '\0')
	{
		tokenize_error("Unclosed quote", &line, line, shell);
	}
	else
		line++;
	return (line);
}

t_token	*word(char **rest, char *line, t_shell *shell)
{
	const char	*start = line;
	char		*word;

	while (*line && !is_metacharacter(*line))
	{
		if (*line == SINGLE_QUOTE_CHAR || *line == DOUBLE_QUOTE_CHAR)
			line = process_quote(line, *line, shell);
		else
			line++;
	}
	word = ft_strndup(start, line - start);
	if (word == NULL)
		fatal_error("strndup");
	*rest = line;
	return (new_token(word, TOKEN_WORD));
}

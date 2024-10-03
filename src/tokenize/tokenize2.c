/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/28 17:30:03 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_new_token(char **line, t_shell *shell)
{
	if (is_metacharacter(**line))
		return (operator(line, *line));
	else if (is_word(*line))
		return (word(line, *line, shell));
	else
	{
		tokenize_error("Unexpected Token", line, *line, shell);
		return (NULL);
	}
}

t_token	*tokenize(char *line, t_shell *shell)
{
	t_token	head;
	t_token	*token;
	t_token	*new_tok;

	shell->syntax_error = false;
	head.next = NULL;
	token = &head;
	while (*line)
	{
		if (consume_blank(&line, line))
			continue ;
		new_tok = create_new_token(&line, shell);
		if (new_tok == NULL)
			break ;
		token->next = new_tok;
		token = new_tok;
	}
	token->next = new_token(NULL, TOKEN_EOF);
	return (head.next);
}

char	**tail_recursive(t_token *token, int nargs, char **argv)
{
	char	**new_argv;

	if (token == NULL || token->kind == TOKEN_EOF)
		return (argv);
	new_argv = malloc((nargs + 2) * sizeof(char *));
	if (new_argv == NULL)
	{
		free(argv);
		fatal_error("malloc");
	}
	ft_memcpy(new_argv, argv, nargs * sizeof(char *));
	free(argv);
	argv = new_argv;
	argv[nargs] = ft_strdup(token->word);
	if (argv[nargs] == NULL)
		fatal_error("strdup");
	argv[nargs + 1] = NULL;
	return (tail_recursive(token->next, nargs + 1, argv));
}

char	**token_list_to_argv(t_token *token)
{
	char	**argv;

	argv = malloc(sizeof(char *));
	if (argv == NULL)
		fatal_error("malloc");
	ft_memset(argv, 0, sizeof(char *));
	return (tail_recursive(token, 0, argv));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 13:24:54 by a.               ###   ########.fr       */
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

bool	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
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

bool	is_metacharacter(char c)
{
	if (is_blank(c))
		return (true);
	return (c && ft_strchr("|&;()<>\n", c));
}

bool	is_word(const char *s)
{
	return (*s && !is_metacharacter(*s));
}

t_token	*operator(char **rest, char *line)
{
	size_t	i;
	char	*op;

	static char *const operators[] = {">>", "<<", "||", "&&", ";;", "<", ">",
		"&", ";", "(", ")", "|", "\n"};
	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
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
	assert_error("Unexpected operator");
}

t_token	*word(char **rest, char *line, t_shell *shell)
{
	const char	*start = line;
	char		*word;

	while (*line && !is_metacharacter(*line))
	{
		if (*line == SINGLE_QUOTE_CHAR)
		{
			line++;
			while (*line && *line != SINGLE_QUOTE_CHAR)
				line++;
			if (*line == '\0')
			{
				tokenize_error("Unclosed single quote", &line, line, shell);
				break ;
			}
			else
				line++;
		}
		else if (*line == DOUBLE_QUOTE_CHAR)
		{
			line++;
			while (*line && *line != DOUBLE_QUOTE_CHAR)
				line++;
			if (*line == '\0')
			{
				tokenize_error("Unclosed double quote", &line, line, shell);
				break ;
			}
			else
				line++;
		}
		else
			line++;
	}
	word = ft_strndup(start, line - start);
	if (word == NULL)
		fatal_error("strndup");
	*rest = line;
	return (new_token(word, TOKEN_WORD));
}

t_token	*tokenize(char *line, t_shell *shell)
{
	t_token	head;
	t_token	*token;

	shell->syntax_error = false; // 構造体内の syntax_error を使用
	head.next = NULL;
	token = &head;
	while (*line)
	{
		if (consume_blank(&line, line))
			continue ;
		else if (is_metacharacter(*line))
			token = token->next = operator(&line, line);
		else if (is_word(line))
			token = token->next = word(&line, line, shell);
		else
			tokenize_error("Unexpected Token", &line, line, shell); // ここでエラーが起きたとき、構造体の値を設定
	}
	token->next = new_token(NULL, TOKEN_EOF);
	return (head.next);
}
char	**tail_recursive(t_token *token, int nargs, char **argv)
{
	char	**new_argv;

	if (token == NULL || token->kind == TOKEN_EOF)
		return (argv);
	// 新しいサイズのメモリを確保
	new_argv = malloc((nargs + 2) * sizeof(char *));
	if (new_argv == NULL)
	{
		free(argv); // メモリ確保が失敗した場合、元のポインタを解放
		fatal_error("malloc");
	}
	// 元のメモリ内容を新しい領域にコピー
	ft_memcpy(new_argv, argv, nargs * sizeof(char *));
	free(argv); // 古いメモリを解放
	argv = new_argv;
	// 次のトークンを追加
	argv[nargs] = ft_strdup(token->word);
	if (argv[nargs] == NULL)
		fatal_error("strdup");
	argv[nargs + 1] = NULL;
	return (tail_recursive(token->next, nargs + 1, argv));
}

char	**token_list_to_argv(t_token *token)
{
	char	**argv;

	argv = malloc(sizeof(char *)); // 1つのポインタ分のメモリを確保
	if (argv == NULL)
		fatal_error("malloc");
	ft_memset(argv, 0, sizeof(char *)); // メモリをゼロ初期化
	return (tail_recursive(token, 0, argv));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/25 21:08:43 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_node(t_node *node)
{
	if (node == NULL)
		return ;
	free_token(node->args);
	free_token(node->filename);
	free_token(node->delimiter);
	free_node(node->redirects);
	free_node(node->next);
	free_node(node->command);
	free(node);
}

void	free_token(t_token *token)
{
	if (token == NULL)
		return ;
	if (token->word)
		free(token->word);
	free_token(token->next);
	free(token);
}

void	free_argv(char **argv)
{
	int i;
	if (argv == NULL)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
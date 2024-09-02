#include <stdlib.h>
#include "minishell.h"
#include <stdio.h>

void free_node(t_node *node)
{
    if(node == NULL)
        return;
    free_token(node->args);
	free_token(node->filename);
    free_token(node->delimiter);
	free_node(node->redirects);
    free_node(node->next);
    free(node);
}

void free_token(t_token *token)
{
    if(token == NULL)
        return;
    if(token->word)
        free(token->word);
    free_token(token->next);
    free(token);
}

void free_argv(char** argv)
{
    int i;
    if(argv == NULL)
        return;
    i = 0;
    while(argv[i])
    {
        free(argv[i]);
        i++;
    }
    free(argv);
}
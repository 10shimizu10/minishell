#include <stdlib.h>
#include "minishell.h"

void free_token(Token *token)
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
#include <stdlib.h>
#include "minishell.h"

#include <string.h>

t_node *parse(t_token *token)
{
    t_node *node;

    node = new_node(ND_SIMPLE_CMD);
    while(token && !at_eof(token))
    {
        if(token->kind == TOKEN_WORD)
        {
            append_token(&node->args, tokendup(token));
            token = token->next;
        }
        else
        parse_error("Unexpected Token", &token, token);
    }
    return node;
}

bool at_eof(t_token *token)
{
    return (token->kind == TOKEN_EOF);
}

t_node *new_node(t_node_type type)
{
    t_node *node;

    node = malloc(sizeof(*node));
    if (node == NULL)
        fatal_error("malloc");
    memset(node, 0, sizeof(*node));

    node->kind = type;
    return (node);
}

t_token *tokendup(t_token *token)
{
    char *word;

    word = strdup(token->word);
    if(word == NULL)
        fatal_error("strdup");
    return new_token(word, token->kind);
}

void append_token(t_token **tokens, t_token *token)
{
    if(*tokens == NULL)
    {
        *tokens = token;
        return ;
    }
    append_token(&(*tokens)->next, token);
}
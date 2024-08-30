#include <stdlib.h>
#include "minishell.h"

#include <string.h>

t_node *parse(Token *token)
{
    t_node *node;

    node = new_node(ND_SIMPLE_CMD);
    while(token && !at_eof(token))
    {
        if(token->kind == TOKEN_WORD)
            append_token(&node->args, tokendup(token));
        else
            todo("Implement parser");
        token = token->next;
    }
    return node;
}

bool at_eof(Token *token)
{
    return (token->kind == TOKEN_EOF);
}

t_node *new_node(t_node_kind kind)
{
    t_node *node;

    node = malloc(sizeof(*node));
    if (node == NULL)
        fatal_error("malloc");
    memset(node, 0, sizeof(*node));

    node->kind = kind;
    return (node);
}

Token *tokendup(Token *token)
{
    char *word;

    word = strdup(token->word);
    if(word == NULL)
        fatal_error("strdup");
    return new_token(word, token->kind);
}

void append_token(Token **tokens, Token *token)
{
    if(*tokens == NULL)
    {
        *tokens = token;
        return ;
    }
    append_token(&(*tokens)->next, token);
}
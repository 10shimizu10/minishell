#include <stdlib.h>
#include "minishell.h"

#include <string.h>

void append_char(char **s, char c)
{
    size_t size;
    char *new;

    size = 2;
    if(*s)
        size += strlen(*s);
    new = malloc(size);
    if(new == NULL)
        fatal_error("malloc");
    if(*s)
        strlcpy(new, *s, size);
    new[size - 2] = c;
    new[size - 1] = '\0';
    if(*s)
        free(*s);
    *s = new;
}

void quote_removal(Token *token)
{
    char *new_word;
    char *p;

    if(token == NULL || token->kind != TOKEN_WORD || token->word == NULL)
        return ;
    p = token->word;
    new_word = NULL;
    while(*p && !is_metacharacter(*p))
    {
        if(*p == SINGLE_QUOTE_CHAR)
        {
            p++;
            while(*p != SINGLE_QUOTE_CHAR)
            {
                if(*p == '\0')
                    assert_error("Unclosed single quote");
                append_char(&new_word, *p++);
            }
            p++;
        }
        else if(*p == DOUBLE_QUOTE_CHAR)
        {
            p++;
            while(*p != DOUBLE_QUOTE_CHAR)
            {
                if(*p == '\0')
                    assert_error("Unclosed single quote");
                append_char(&new_word, *p++);
            }
            p++;
        }
        else
            append_char(&new_word, *p++);
    }
    free(token->word);
    token->word = new_word;
    quote_removal(token->next);
}

void expand(Token *token)
{
    quote_removal(token);
}
#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdbool.h>


// error.c
void	todo(const char *msg) __attribute__((noreturn));
void	fatal_error(const char *msg) __attribute__((noreturn));
void	assert_error(const char *msg) __attribute__((noreturn));
void	err_exit(const char *location, const char *msg, int status) __attribute__((noreturn));

// tokenize.c
typedef enum {
    TOKEN_WORD,
    TOKEN_RESERVED,
    TOKEN_OP,
    TOKEN_EOF
} Token_Type;


typedef struct Token {
    char *word;
    Token_Type kind;
    struct Token *next;
} Token;

Token *tokenize(char *line);
char **token_list_to_argv(Token *token);

// expand.c
void expand(Token *token);

// destructor.c
void free_token(Token *token);
void free_argv(char **argv);

#endif
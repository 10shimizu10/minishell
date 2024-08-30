#ifndef MINISHELL_H
#define MINISHELL_H

#include <stddef.h>
#include <stdbool.h>


// error.c
# define ERROR_TOKENIZE 258
extern bool syntax_error;
void	todo(const char *msg) __attribute__((noreturn));
void	fatal_error(const char *msg) __attribute__((noreturn));
void	assert_error(const char *msg) __attribute__((noreturn));
void	err_exit(const char *location, const char *msg, int status) __attribute__((noreturn));
void tokenize_error(const char *location, char **rest, char *line);

// tokenize.c
typedef enum {
    TOKEN_WORD,
    TOKEN_RESERVED,
    TOKEN_OP,
    TOKEN_EOF
} TokenType;


typedef struct Token {
    char *word;
    TokenType kind;
    struct Token *next;
} Token;

//token.c
# define SINGLE_QUOTE_CHAR '\''
# define DOUBLE_QUOTE_CHAR '"'

Token *tokenize(char *line);
char **token_list_to_argv(Token *token);
Token	*new_token(char *word, TokenType kind);
bool   	is_blank(char c);
bool   	consume_blank(char **rest, char *line);
bool   	startswith(const char *s, const char *keyword);
bool   	is_operator(const char *s);
bool   	is_metacharacter(char c);
bool   	is_word(const char *s);
Token	*operator(char **rest, char *line);
Token	*word(char **rest, char *line);

// expand.c
void expand(Token *token);

// destructor.c
void free_token(Token *token);
void free_argv(char **argv);

#endif
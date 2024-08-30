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

enum e_node_kind {
    ND_SIMPLE_CMD,
};

typedef enum e_node_kind t_node_kind;

typedef struct s_node t_node;
struct s_node
{
    Token *args;
    t_node_kind kind;
    t_node *next;
};

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
void expand(t_node *node);

// destructor.c
void free_node(t_node *node);
void free_token(Token *token);
void free_argv(char **argv);

//parse.c
t_node *parse(Token *token);
bool at_eof(Token *token);
t_node *new_node(t_node_kind kind);
void append_token(Token **tokens, Token *token);
Token *tokendup(Token *token);

#endif
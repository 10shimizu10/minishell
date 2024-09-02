#ifndef MINISHELL_H
#define MINISHELL_H

#include <stddef.h>
#include <stdbool.h>


#define ERROR_TOKENIZE 258
#define ERROR_PARSE 258;
# define ERROR_OPEN_REDIR 1
# define SINGLE_QUOTE_CHAR '\''
# define DOUBLE_QUOTE_CHAR '"'

extern bool syntax_error;

typedef enum e_token_type{
    TOKEN_WORD,
    TOKEN_RESERVED,
    TOKEN_OP,
    TOKEN_EOF
} t_token_type;

typedef struct s_token t_token;
typedef struct s_token
{
    char *word;
    t_token_type kind;
    t_token *next;
}t_token;

typedef enum e_node_type {
    ND_PIPELINE,
    ND_SIMPLE_CMD,
	ND_REDIR_OUT,
	ND_REDIR_IN,
	ND_REDIR_APPEND,
    ND_REDIR_HEREDOC,
} t_node_type;


typedef struct s_node t_node;
typedef struct s_node
{
    t_node_type kind;
    t_node *next;
	// CMD
	t_token		*args;
	t_node		*redirects;
	// REDIR
	int			targetfd;
	t_token		*filename;
    t_token *delimiter;
	int			filefd;
	int			stashed_targetfd;
	// PIPELINE
	int			inpipe[2];
	int			outpipe[2];
	t_node		*command;
} t_node;

// error.c
void	todo(const char *msg) __attribute__((noreturn));
void	fatal_error(const char *msg) __attribute__((noreturn));
void	assert_error(const char *msg) __attribute__((noreturn));
void	err_exit(const char *location, const char *msg, int status) __attribute__((noreturn));
void    tokenize_error(const char *location, char **rest, char *line);
void    parse_error(const char *location, t_token **rest, t_token * token);
void    xperror(const char *location);

// tokenize.c
t_token *tokenize(char *line);
char **token_list_to_argv(t_token *token);
t_token	*new_token(char *word, t_token_type kind);
bool   	is_blank(char c);
bool   	consume_blank(char **rest, char *line);
bool   	startswith(const char *s, const char *keyword);
bool   	is_operator(const char *s);
bool   	is_metacharacter(char c);
bool   	is_word(const char *s);
t_token	*operator(char **rest, char *line);
t_token	*word(char **rest, char *line);

// expand.c
void expand(t_node *node);

// destructor.c
void free_node(t_node *node);
void free_token(t_token *token);
void free_argv(char **argv);

//parse.c
t_node *parse(t_token *token);
void	append_command_element(t_node *command, t_token **rest, t_token *tok);
bool at_eof(t_token *token);
t_node *new_node(t_node_type kind);
void append_token(t_token **tokens, t_token *token);
t_token *token_dup(t_token *token);

// redirect.c
int		open_redir_file(t_node *node);
void	do_redirect(t_node *redirects);
void	reset_redirect(t_node *redirects);

// pipe.c
void	prepare_pipe(t_node *node);
void	prepare_pipe_child(t_node *node);
void	prepare_pipe_parent(t_node *node);

#endif
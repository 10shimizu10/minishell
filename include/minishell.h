#ifndef MINISHELL_H
# define MINISHELL_H

# include "minishell.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <unistd.h>

# define ERROR_TOKENIZE 258
# define ERROR_PARSE 258;
# define ERROR_OPEN_REDIR 1
# define SINGLE_QUOTE_CHAR '\''
# define DOUBLE_QUOTE_CHAR '"'
# define ERROR_PREFIX "minishell: "

typedef struct s_item	t_item;
struct					s_item
{
	char				*name;
	char				*value;
	t_item				*next;
};

typedef struct s_map	t_map;
struct					s_map
{
	t_item				item_head;
};
extern t_map			*envmap;

typedef struct s_shell
{
	int					last_status;
	bool				syntax_error;
	bool				readline_interrupted;
	t_map				*envmap;
}						t_shell;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_RESERVED,
	TOKEN_OP,
	TOKEN_EOF
}						t_token_type;

typedef struct s_token	t_token;
typedef struct s_token
{
	char				*word;
	t_token_type		kind;
	t_token				*next;
}						t_token;

typedef enum e_node_type
{
	ND_PIPELINE,
	ND_SIMPLE_CMD,
	ND_REDIR_OUT,
	ND_REDIR_IN,
	ND_REDIR_APPEND,
	ND_REDIR_HEREDOC,
}						t_node_type;

typedef struct s_node	t_node;
typedef struct s_node
{
	t_node_type			kind;
	t_node				*next;
	// CMD
	t_token				*args;
	t_node				*redirects;
	// REDIR
	int					targetfd;
	t_token				*filename;
	t_token				*delimiter;
	bool				is_delim_unquoted;
	int					filefd;
	int					stashed_targetfd;
	// PIPELINE
	int					inpipe[2];
	int					outpipe[2];
	t_node				*command;
}						t_node;

// error.c
void					perror_prefix(void);
void					todo(const char *msg) __attribute__((noreturn));
void					fatal_error(const char *msg) __attribute__((noreturn));
void					assert_error(const char *msg) __attribute__((noreturn));
void					err_exit(const char *location, const char *msg,
							int status) __attribute__((noreturn));
void					tokenize_error(const char *location, char **rest,
							char *line, t_shell *shell);
void					parse_error(const char *location, t_token **rest,
							t_token *token, t_shell *shell);
void					xperror(const char *location);
void					builtin_error(const char *func, const char *name,
							const char *err);

// tokenize.c
t_token					*tokenize(char *line, t_shell *shell);
char					**token_list_to_argv(t_token *token);
t_token					*new_token(char *word, t_token_type kind);
bool					is_blank(char c);
bool					consume_blank(char **rest, char *line);
bool					startswith(const char *s, const char *keyword);
bool					is_operator(const char *s);
bool					is_metacharacter(char c);
bool					is_word(const char *s);
t_token					*operator(char **rest, char *line);
t_token					*word(char **rest, char *line, t_shell *shell);

// expand.c
void					expand(t_node *node, t_shell *shell);
char					*expand_heredoc_line(char *line, t_shell *shell);

// destructor.c
void					free_node(t_node *node);
void					free_token(t_token *token);
void					free_argv(char **argv);

// parse.c
t_node					*parse(t_token *token);
void					append_command_element(t_node *command, t_token **rest,
							t_token *tok);
bool					at_eof(t_token *token);
t_node					*new_node(t_node_type kind);
void					append_token(t_token **tokens, t_token *token);
t_token					*token_dup(t_token *token);

// redirect.c
int						stashfd(int fd);
int						read_heredoc(const char *delimiter,
							bool is_delim_unquoted, t_shell *shell);
int						open_redir_file(t_node *node, t_shell *shell);
void					do_redirect(t_node *redirects);
void					reset_redirect(t_node *redirects);

// pipe.c
void					prepare_pipe(t_node *node);
void					prepare_pipe_child(t_node *node);
void					prepare_pipe_parent(t_node *node);

// exec.c
int						exec(t_node *node, t_shell *shell);
char					*search_path(const char *filename, t_shell *shell);
void					validate_access(const char *path, const char *filename);
pid_t					exec_pipeline(t_node *node, t_shell *shell);
int						wait_pipeline(pid_t last_pid);

// signal.h

void					reset_sig(int signum);
void					setup_signal(void);

// builtin.c
bool					is_builtin(t_node *node);
int						exec_builtin(t_node *node, t_shell *shell);

// builtin_exit.c
bool					is_numeric(char *s);
int						builtin_exit(char **argv, t_shell *shell);

// builtin_export.c
int						builtin_export(char **argv, t_shell *shell);

// builtin_unset.c
int						builtin_unset(char **argv, t_shell *shell);

// builtin_env.c
int						builtin_env(char **argv, t_shell *shell);

// builtin_cd.c
char					*resolve_pwd(char *oldpwd, char *path);
int						builtin_cd(char **argv, t_shell *shell);

// builtin_echo.c
int						builtin_echo(char **argv);

// builtin_pwd.c
int						builtin_pwd(char **argv, t_shell *shell);

// map.c
t_item					*item_new(char *name, char *value);
char					*item_get_string(t_item *item);
t_map					*map_new(void);
char					*map_get(t_map *map, const char *name);
int						map_put(t_map *map, const char *string,
							bool allow_empty_value);
int						map_set(t_map *map, const char *name,
							const char *value);
int						map_unset(t_map *map, const char *name);
size_t					map_len(t_map *map, bool count_null_value);
void					map_printall(t_map *map);
t_item					*find_item(t_item *head, const char *name);
void					update_item(t_item *item, const char *value);
// env.c
char					*xgetenv(const char *name, t_shell *shell);
void					initenv(t_shell *shell);
char					**get_environ(t_map *map);

// ft_utils
void					*ft_memset(void *b, int c, size_t len);
size_t					ft_strlen(const char *str);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
char					*ft_strcat(char *dest, const char *src);
char					*ft_strncat(char *dest, const char *src, size_t n);
size_t					ft_strlcpy(char *dest, const char *src, size_t size);
char					*ft_strdup(const char *s1);
char					*ft_strndup(const char *s, size_t n);
int						ft_strcmp(const char *s1, const char *s2);
size_t					ft_strlcat(char *dst, const char *src, size_t dstsize);
char					*ft_strchr(const char *s, int c);
long					ft_strtol(const char *str, char **endptr, int base);
int						ft_printf(const char *format, ...);
void					*ft_memcpy(void *dst, const void *src, size_t n);
int						ft_memcmp(const void *s1, const void *s2, size_t n);
bool					is_digit(char c);
bool					is_blank(char c);
bool					is_identifier(const char *s);
bool					is_alpha_under(char c);
bool					is_alpha_num_under(char c);
char					*ft_strncpy(char *dst, const char *src, size_t len);

bool					equal_op(t_token *tok, char *op);
void					append_node(t_node **node, t_node *elm);
t_node					*pipeline(t_token **rest, t_token *token);
t_node					*simple_command(t_token **rest, t_token *token);

#endif
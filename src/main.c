#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "minishell.h"

//パスを探す
char *search_path(const char* filename)
{
    char path[PATH_MAX];
    char *value;
    char *end;

    value = getenv("PATH");
    while(*value)
    {
        memset(path, 0, PATH_MAX);
        end = strchr(value, ':');
        if(end)
            strncpy(path, value, end - value);
        else
            strlcpy(path, value, PATH_MAX);
        strlcat(path, "/", PATH_MAX);
        strlcat(path, filename, PATH_MAX);
        if(access(path, X_OK) == 0)
        {
            char *dup;

            dup = strdup(path);
            if(dup == NULL)
                fatal_error("strdup");
            return dup;
        }
        if(end == NULL)
            return NULL;
        value = end + 1;
    }
    return NULL;
}

void validate_access(const char *path, const char *filename)
{
    if(path == NULL)
        err_exit(filename, "command not found", 127);
    if(access(path, F_OK) < 0)
        err_exit(filename, "command not found", 127);
}

//ファイルの実行
int	exec_cmd(t_node *node)
{
    extern char **environ;
	char		*path;
    pid_t pid;
    int wstatus;
	char		**argv;

    pid = fork();
    if(pid < 0)
        fatal_error("fork");
    else if(pid == 0)
    {
		argv = token_list_to_argv(node->args);
		path = argv[0];
        if(strchr(path, '/') == NULL)
            path = search_path(path);
        validate_access(path, argv[0]);
        execve(path, argv, environ);
        fatal_error("execve");
    }
    else
    {
        wait(&wstatus);
        return (WEXITSTATUS(wstatus));
    }
}

int	exec(t_node *node)
{
	int	status;

	if (open_redir_file(node->redirects) < 0)
		return (ERROR_OPEN_REDIR);
	do_redirect(node->redirects);
	status = exec_cmd(node);
	reset_redirect(node->redirects);
	return (status);
}

void interpret(char *line, int *stat_loc)
{
    t_token *token;
    t_node *node;

    token = tokenize(line);
	if (at_eof(token))
        ;
    else if(syntax_error)
        *stat_loc = ERROR_TOKENIZE;
    else
    {
        node = parse(token);
		if (syntax_error)
			*stat_loc = ERROR_PARSE;
		if (!syntax_error)
		{
			expand(node);
			*stat_loc = exec(node);
		}
        free_node(node);
    }
    free_token(token);
}

int main()
{
    int status;
    char* line;

    rl_outstream = stderr;
    status = 0;

    while(1)
    {
        line = readline("minishell$ ");
        if(line == NULL)
            break ;

        if(*line)
            add_history(line);
        interpret(line, &status);
        free(line);
    }
    //write_history("history.txt");
    exit(status);
}
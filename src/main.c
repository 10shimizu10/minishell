#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>


//noreturn 下記の関数にたどり着いた時、元の関数はreturnしないことをコンパイラに伝える
void fatal_error(const char *msg) __attribute__((noreturn));
void err_exit(const char *location, const char *msg, int status) __attribute__((noreturn));

void err_exit(const char *location, const char *msg, int status)
{
    const char *prefix = "minishell: ";
    const char *separator = ": ";
    const char *newline = "\n";

    write(STDERR_FILENO, prefix, strlen(prefix));
    write(STDERR_FILENO, location, strlen(location));
    write(STDERR_FILENO, separator, strlen(separator));
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, newline, 1);
    exit(status);
}

void fatal_error(const char *msg)
{
    const char *prefix = "Fatal Error: ";
    write(STDERR_FILENO, prefix, strlen(prefix));
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, "\n", 1);
    exit(1);
}

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
int exec(char *argv[])
{
    extern char **environ;
    const char *path = argv[0];
    pid_t pid;
    int wstatus;

    pid = fork();
    if(pid < 0)
        fatal_error("fork");
    else if(pid == 0)
    {
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

int interpret(char *line)
{
    int status;
    char *argv[] = {line, NULL};

    status = exec(argv);
    return status;
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
        status = interpret(line);
        free(line);
    }
    write_history("history.txt");
    exit(status);
}
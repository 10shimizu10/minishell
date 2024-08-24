#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

void fatal_error(const char *msg) __attribute__((noreturn));

void fatal_error(const char *msg)
{
    const char *prefix = "Fatal Error: ";
    write(STDERR_FILENO, prefix, strlen(prefix));
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, "\n", 1);
    exit(1);
}

int interpret(char *line)
{
    extern char **environ;
    char *argv[] = {line, NULL};
    pid_t pid;
    int wstatus;

    pid = fork();
    if(pid < 0)
        fatal_error("fork");
    else if(pid == 0)
    {
        execve(line, argv, environ);
        fatal_error("execve");
    }
    else
    {
        wait(&wstatus);
        return (WEXITSTATUS(wstatus));
    }
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
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "minishell.h"

bool syntax_error = false;

void fatal_error(const char *msg)
{
    const char *prefix = "Fatal Error: ";
    write(STDERR_FILENO, prefix, strlen(prefix));
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, "\n", 1);
    exit(1);
}

void tokenize_error(const char *location, char **rest, char *line)
{
    syntax_error = true;

    write(STDERR_FILENO, "minishell: syntax error near ", 29);
    write(STDERR_FILENO, location, strlen(location));
    write(STDERR_FILENO, "\n", 1);
    while (*line)
        line++;
    *rest = line;
}

void assert_error(const char *msg)
{
    const char *prefix = "Assert Error: ";
    write(STDERR_FILENO, prefix, strlen(prefix));
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, "\n", 1);
    exit(255);
}

void err_exit(const char *location, const char *msg, int status)
{
    const char *prefix = "minishell: ";
    write(STDERR_FILENO, prefix, strlen(prefix));
    write(STDERR_FILENO, location, strlen(location));
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, "\n", 1);
    exit(status);
}

void todo(const char *msg)
{
    const char *prefix = "TODO: ";
    write(STDERR_FILENO, prefix, strlen(prefix));
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, "\n", 1);
    exit(255);
}

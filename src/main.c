#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
    char* line;

    while(1)
    {
        line = readline("minishell$ ");
        if(line == NULL)
            break ;

        if(*line)
            add_history(line);
        free(line);
    }
    exit(0);
}
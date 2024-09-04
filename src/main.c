#include <stdlib.h>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "minishell.h"

int	last_status;

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
    char* line;

    rl_outstream = stderr;
	setup_signal();
    last_status = 0;

    while(1)
    {
        line = readline("minishell$ ");
        if(line == NULL)
            break ;

        if(*line)
            add_history(line);
        interpret(line, &last_status);
        free(line);
    }
    //write_history("history.txt");
    exit(last_status);
}
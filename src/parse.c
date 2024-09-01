#include <stdlib.h>
#include "minishell.h"

#include <unistd.h>
#include <string.h>


bool	equal_op(t_token *tok, char *op);
void	append_node(t_node **node, t_node *elm);


t_node *parse(t_token *token)
{
    t_node *node;

    node = new_node(ND_SIMPLE_CMD);
	append_command_element(node, &token, token);
    while(token && !at_eof(token))
    	append_command_element(node, &token, token);
    return node;
}

t_node	*redirect_out(t_token **rest, t_token *token)
{
	t_node	*node;

	node = new_node(ND_REDIR_OUT);
	node->filename = token_dup(token->next);
	node->targetfd = STDOUT_FILENO;
	*rest = token->next->next;
	return (node);
}

void	append_command_element(t_node *command, t_token **rest, t_token *token)
{
	if (token->kind == TOKEN_WORD)
	{
		append_token(&command->args, token_dup(token));
		token = token->next;
	}
	else if (equal_op(token, ">") && token->next->kind == TOKEN_WORD)
		append_node(&command->redirects, redirect_out(&token, token));
	else
		todo("append_command_element");
	*rest = token;
}

bool at_eof(t_token *token)
{
    return (token->kind == TOKEN_EOF);
}

bool	equal_op(t_token *token, char *op)
{
	if (token->kind != TOKEN_OP)
		return (false);
	return (strcmp(token->word, op) == 0);
}

t_node *new_node(t_node_type type)
{
    t_node *node;

    node = malloc(sizeof(*node));
    if (node == NULL)
        fatal_error("malloc");
    memset(node, 0, sizeof(*node));

    node->kind = type;
    return (node);
}

t_token *token_dup(t_token *token)
{
    char *word;

    word = strdup(token->word);
    if(word == NULL)
        fatal_error("strdup");
    return new_token(word, token->kind);
}

void	append_token(t_token **token, t_token *elm)
{
	if (*token == NULL)
	{
		*token = elm;
		return ;
	}
	append_token(&(*token)->next, elm);
}


void	append_node(t_node **node, t_node *elm)
{
	if (*node == NULL)
	{
		*node = elm;
		return ;
	}
	append_node(&(*node)->next, elm);
}
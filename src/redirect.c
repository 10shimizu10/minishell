// #include <fcntl.h>
// #include <unistd.h>
// #include "minishell.h"

// int	stashfd(int fd)
// {
// 	int	stashfd;

// 	stashfd = fcntl(fd, F_DUPFD, 10);
// 	if (stashfd < 0)
// 		fatal_error("fcntl");
// 	if (close(fd) < 0)
// 		fatal_error("close");
// 	return (stashfd);
// }
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <errno.h>
#include <sys/stat.h>
#include <errno.h>
#include "minishell.h"
#include <string.h>

bool	readline_interrupted = false;

static bool	is_valid_fd(int fd)
{
	struct stat	st;

	if (fd < 0)
		return (false);
	errno = 0;
	if (fstat(fd, &st) < 0 && errno == EBADF)
		return (false);
	return (true);
}

int	stashfd(int fd)
{
	int	stashfd;

	if (!is_valid_fd(fd))
	{
		errno = EBADF;
		return (-1);
	}
	stashfd = 10;
	while (is_valid_fd(stashfd))
		stashfd++;
	stashfd = dup2(fd, stashfd);
	close(fd);
	return (stashfd);
}

int	read_heredoc(const char *delimiter, bool is_delim_unquoted, t_shell *shell)
{
	char	*line;
	int		pfd[2];

	if (pipe(pfd) < 0)
		fatal_error("pipe");
    readline_interrupted = false;
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (readline_interrupted)
		{
			free(line);
			break ;
		}
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (is_delim_unquoted)
			line = expand_heredoc_line(line, shell);
		write(pfd[1], line, strlen(line));
		write(pfd[1], "\n", 1);
		free(line);
	}
	close(pfd[1]);
	if (readline_interrupted)
	{
		close(pfd[0]);
		return (-1);
	}
	return (pfd[0]);
}

int	open_redir_file(t_node *node, t_shell *shell)
{
	if (node == NULL)
		return (0);
	if (node->kind == ND_PIPELINE)
	{
		if (open_redir_file(node->command, shell) < 0)
			return (-1);
		if (open_redir_file(node->next, shell) < 0)
			return (-1);
		return (0);
	}
	else if (node->kind == ND_SIMPLE_CMD)
		return (open_redir_file(node->redirects, shell));
	else if (node->kind == ND_REDIR_OUT)
		node->filefd = open(node->filename->word, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (node->kind == ND_REDIR_IN)
		node->filefd = open(node->filename->word, O_RDONLY);
	else if (node->kind == ND_REDIR_APPEND)
		node->filefd = open(node->filename->word, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (node->kind == ND_REDIR_HEREDOC)
				node->filefd = read_heredoc(node->delimiter->word, node->is_delim_unquoted, shell);
    else
		assert_error("oopen_redir_file");
	if (node->filefd < 0)
	{
		if (node->kind == ND_REDIR_OUT || node->kind == ND_REDIR_APPEND || node->kind == ND_REDIR_IN)
			xperror(node->filename->word);
		return (-1);
	}
	node->filefd = stashfd(node->filefd);
	return (open_redir_file(node->next, shell));
}

bool is_redirect(t_node *node)
{
    if(node->kind == ND_REDIR_OUT)
        return true;
    else if(node->kind == ND_REDIR_IN)
        return true;
    else if(node->kind == ND_REDIR_APPEND)
        return true;
    else if(node->kind == ND_REDIR_HEREDOC)
        return true;
    return false;
}

void	do_redirect(t_node *redir)
{
	if (redir == NULL)
		return ;
	if (is_redirect(redir))
	{
		redir->stashed_targetfd = stashfd(redir->targetfd);
		dup2(redir->filefd, redir->targetfd);
	}
	else
		assert_error("do_redirect");
	do_redirect(redir->next);
}

// Reset must be done from tail to head
void	reset_redirect(t_node *redir)
{
	if (redir == NULL)
		return ;
	reset_redirect(redir->next);
	if (is_redirect(redir))
	{
		close(redir->filefd);
		close(redir->targetfd);
		dup2(redir->stashed_targetfd, redir->targetfd);
	}
	else
		assert_error("reset_redirect");
}
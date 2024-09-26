#include <unistd.h>
#include <signal.h>
#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t	sig = 0;  // シグナル番号を保持

void	handler(int signum)
{
	sig = signum;  // 受け取ったシグナルを設定
}

void	reset_sig(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;  // デフォルトのシグナル動作に戻す
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error("sigaction");
}

void	ignore_sig(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;  // シグナルを無視
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error("sigaction");
}

void	setup_sigint(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;  // シグナルハンドラを設定
	if (sigaction(SIGINT, &sa, NULL) < 0)
		fatal_error("sigaction");
}

int	check_state(void)
{
	if (sig == 0)
		return (0);  // シグナルが無ければ何もしない
	else if (sig == SIGINT)
	{
		sig = 0;  // シグナルをリセット
		rl_replace_line("", 0);  // 現在の行をリセット
		rl_done = 1;  // readlineを終了させる
		return (0);
	}
	return (0);
}

void	setup_signal(void)
{
	extern int	_rl_echo_control_chars;

	_rl_echo_control_chars = 0;
	rl_outstream = stderr;
	if (isatty(STDIN_FILENO))  // 対話的な端末であるか確認
		rl_event_hook = check_state;  // readlineのイベントフックを設定
	ignore_sig(SIGQUIT);  // SIGQUITを無視
	setup_sigint();  // SIGINTハンドラを設定
}

void	reset_signal(void)
{
	reset_sig(SIGQUIT);  // SIGQUITをデフォルト動作に戻す
	reset_sig(SIGINT);   // SIGINTをデフォルト動作に戻す
}


// #include <unistd.h>
// #include <signal.h>
// #include "minishell.h"
// #include <stdio.h>
// #include <readline/readline.h>
// #include <readline/history.h>


// volatile sig_atomic_t	sig = 0;

// void	handler(int signum)
// {
// 	sig = signum;
// }

// void	reset_sig(int signum)
// {
// 	struct sigaction	sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	sa.sa_handler = SIG_DFL;
// 	if (sigaction(signum, &sa, NULL) < 0)
// 		fatal_error("sigaction");
// }

// void	ignore_sig(int signum)
// {
// 	struct sigaction	sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	sa.sa_handler = SIG_IGN;
// 	if (sigaction(signum, &sa, NULL) < 0)
// 		fatal_error("sigaction");
// }

// void	setup_sigint(void)
// {
// 	struct sigaction	sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	sa.sa_handler = handler;
// 	if (sigaction(SIGINT, &sa, NULL) < 0)
// 		fatal_error("sigaction");
// }

// #include <stdio.h>
// #include <readline/readline.h>

// int	check_state(void)
// {
// 	if (sig == 0)
// 		return (0);
// 	else if (sig == SIGINT)
// 	{
// 		sig = 0;
// 		readline_interrupted = true;
// 		rl_replace_line("", 0);
// 		rl_done = 1;
// 		return (0);
// 	}
// 	return (0);
// }

// void	setup_signal(void)
// {
// 	extern int	_rl_echo_control_chars;

// 	_rl_echo_control_chars = 0;
// 	rl_outstream = stderr;
// 	if (isatty(STDIN_FILENO))
// 		rl_event_hook = check_state;
// 	ignore_sig(SIGQUIT);
// 	setup_sigint();
// }

// void	reset_signal(void)
// {
// 	reset_sig(SIGQUIT);
// 	reset_sig(SIGINT);
// }
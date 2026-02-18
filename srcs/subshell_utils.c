/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 12:19:56 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/18 12:21:26 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_sig_msg(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
}

int	handle_subshell(t_cmd *cmd, t_shell *sh)
{
	pid_t	pid;
	int		status;
	int		st;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), setup_signals(), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (setup_redirection(cmd, sh) == -1)
			exit(1);
		st = execute_ast(cmd->subshell, sh);
		exit(st);
	}
	status = 0;
	while (waitpid(pid, &status, 0) < 0 && errno == EINTR)
		;
	print_sig_msg(status);
	if (WIFEXITED(status))
		sh->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_status = 128 + WTERMSIG(status);
	setup_signals();
	return (sh->last_status);
}

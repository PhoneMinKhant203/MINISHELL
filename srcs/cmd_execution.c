/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 21:08:30 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/16 19:58:02 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	child_exec_binary(t_cmd *cmd, t_shell *sh, int i)
{
	char	*path;
	char	**env_arr;
	int		err;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!cmd->args || !cmd->args[i] || cmd->args[i][0] == '\0')
		exit(127);
	path = resolve_path(cmd, sh, i);
	if (is_directory(path))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[i], 2);
		ft_putendl_fd(": Is a directory", 2);
		free(path);
		exit(126);
	}
	env_arr = env_to_array(sh->env);
	if (!env_arr)
	{
		perror("minishell");
		free(path);
		exit(1);
	}
	execve(path, &cmd->args[i], env_arr);
	err = errno;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[i], 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(err), 2);
	free(path);
	free2p(env_arr);
	if (err == EACCES || err == EISDIR)
		exit(126);
	exit(127);
}

static void	child_process(t_cmd *cmd, t_shell *sh, int fdin, int p_fd[2])
{
	int	i;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (fdin != STDIN_FILENO)
	{
		dup2(fdin, STDIN_FILENO);
		close(fdin);
	}
	if (cmd->next)
	{
		close(p_fd[0]);
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[1]);
	}
	if (setup_redirection(cmd, sh) == -1)
		exit(1);
	i = 0;
	if (is_builtin(&cmd->args[i]))
		exit(exe_builtin(&cmd->args[i], sh));
	child_exec_binary(cmd, sh, i);
}

void	start_executor(t_cmd *cmds, t_shell *sh)
{
	int		fd_pipe[2];
	int		fd_in;
	pid_t	last_pid;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	fd_in = STDIN_FILENO;
	last_pid = -1;
	while (cmds)
	{
		if (cmds->next && pipe(fd_pipe) == -1)
			return (perror("minishell: pipe"), setup_signals());
		last_pid = fork();
		if (last_pid == -1)
			return (perror("minishell: fork"), setup_signals());
		if (last_pid == 0)
			child_process(cmds, sh, fd_in, fd_pipe);
		update_parent_fds(&fd_in, fd_pipe, cmds->next);
		cmds = cmds->next;
	}
	wait_all_children(sh, last_pid);
	setup_signals();
}

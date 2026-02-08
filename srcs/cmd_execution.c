/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 21:08:30 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/08 17:00:06 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_exec_binary(t_cmd *cmd, t_shell *sh)
{
	char		*path;
	char		**env_arr;
	int			i;

	i = 0;
	while (cmd->args[i] && cmd->args[i][0] == '\0')
		i++;
	if (!cmd->args[i])
		exit(0);
	path = find_path(cmd->args[i], sh->env);
	if (!path)
		exe_error(cmd->args[i]);
	env_arr = env_to_array(sh->env);
	if (!env_arr)
		exit(1);
	if (execve(path, &cmd->args[i], env_arr) == -1)
	{
		perror("minishell: execve");
		exit(126);
	}
}

static void	child_process(t_cmd *cmd, t_shell *sh, int fdin, int p_fd[2])
{
	int	i;

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
	if (setup_redirection(cmd) == -1)
		exit(1);
	i = 0;
	while (cmd->args[i] && cmd->args[i][0] == '\0')
		i++;
	if (!cmd->args[i])
		exit(0);
	if (is_builtin(cmd->args[i]))
		exit(exe_builtin(&cmd->args[i], sh));
	else
		child_exec_binary(cmd, sh);
}

void	start_executor(t_cmd *cmds, t_shell *sh)
{
	int		fd_pipe[2];
	int		fd_in;
	pid_t	pid;

	fd_in = STDIN_FILENO;
	while (cmds)
	{
		if (cmds->next && pipe(fd_pipe) == -1)
			return (perror("minishell: pipe"));
		pid = fork();
		if (pid == -1)
			return (perror("minishell: fork"));
		if (pid == 0)
			child_process(cmds, sh, fd_in, fd_pipe);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (cmds->next)
		{
			close(fd_pipe[1]);
			fd_in = fd_pipe[0];
		}
		cmds = cmds->next;
	}
	wait_all_children(sh);
}

void	wait_all_children(t_shell *sh)
{
	int	status;
	int	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		if (WIFEXITED(status))
			sh->last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			sh->last_status = 128 + WTERMSIG(status);
		pid = wait(&status);
	}
}

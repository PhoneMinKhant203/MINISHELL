/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 21:08:30 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/18 16:45:11 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_error(char *path, char **args, char **env_arr)
{
	int	err;

	err = errno;
	print_err(args[0], NULL, strerror(err));
	if (path)
		free(path);
	if (env_arr)
		free2p(env_arr);
	if (err == EACCES || err == EISDIR)
		exit(126);
	exit(127);
}

static void	child_exec_binary(t_cmd *cmd, t_shell *sh, int i)
{
	char	*path;
	char	**env_arr;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!cmd->args || !cmd->args[i] || cmd->args[i][0] == '\0')
		exit(127);
	path = resolve_path(cmd, sh, i);
	if (is_directory(path))
	{
		print_err(cmd->args[i], NULL, "Is a directory");
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
	handle_error(path, &cmd->args[i], env_arr);
}

static void	child_process(t_cmd *cmd, t_shell *sh, int fdin, int p_fd[2])
{
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
	if (cmd->subshell)
		exit(execute_ast(cmd->subshell, sh));
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (cmd->args && is_builtin(cmd->args))
		exit(exe_builtin(cmd->args, sh));
	child_exec_binary(cmd, sh, 0);
}

static int	handle_fork(t_cmd *cmd, t_shell *sh, int *fdin, int fd_p[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		if (cmd->next)
		{
			close(fd_p[0]);
			close(fd_p[1]);
		}
		if (*fdin != STDIN_FILENO)
			close(*fdin);
		perror("minishell: fork");
		return (-1);
	}
	if (pid == 0)
		child_process(cmd, sh, *fdin, fd_p);
	update_parent_fds(fdin, fd_p, cmd->next);
	return (pid);
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
		{
			if (fd_in != STDIN_FILENO)
				close(fd_in);
			perror("minishell: pipe");
			break ;
		}
		last_pid = handle_fork(cmds, sh, &fd_in, fd_pipe);
		if (last_pid == -1)
			break ;
		cmds = cmds->next;
	}
	wait_all_children(sh, last_pid);
	setup_signals();
}

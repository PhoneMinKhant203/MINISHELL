/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 21:08:30 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/12 17:14:06 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_directory(const char *p)
{
	struct stat	st;

	if (!p)
		return (0);
	if (stat(p, &st) == -1)
		return (0);
	return (S_ISDIR(st.st_mode));
}

/*
if (!path || path[0] == '\0' || !ft_strncmp(cmd->args[i], ".", 2)
		|| !ft_strncmp(cmd->args[i], "..", 3)
		|| !ft_strncmp(path, "IS_DIR", 7))
		exe_error(cmd->args[i], path);
*/
void	child_exec_binary(t_cmd *cmd, t_shell *sh, int i)
{
	char	*path;
	char	**env_arr;
	int		err;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	(void)sh;
	if (!cmd->args || !cmd->args[i] || cmd->args[i][0] == '\0')
	{
		ft_putendl_fd("minishell: : command not found", 2);
		exit(127);
	}
	if (ft_strchr(cmd->args[i], '/'))
		path = ft_strdup(cmd->args[i]);
	else
		path = find_path(cmd->args[i], sh->env);
	if (!path || path[0] == '\0' || ft_strncmp(path, "IS_DIR", 7) == 0)
		exe_error(cmd->args[i], path);
	if (ft_strchr(cmd->args[i], '/') && is_directory(path))
	{
		errno = EISDIR;
		perror(cmd->args[i]);
		free(path);
		exit(126);
	}
	env_arr = env_to_array(sh->env);
	execve(path, &cmd->args[i], env_arr);
	err = errno;
	errno = err;
	perror(cmd->args[i]);
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
	if (setup_redirection(cmd) == -1)
		exit(1);

	i = 0;
	while (cmd->args && cmd->args[i] && cmd->args[i][0] == '\0')
		i++;
	if (!cmd->args || !cmd->args[i])
		exit(0);
	if (is_builtin(&cmd->args[i]))
		exit(exe_builtin(&cmd->args[i], sh));
	child_exec_binary(cmd, sh, i);
}


void	start_executor(t_cmd *cmds, t_shell *sh)
{
	int		fd_pipe[2];
	int		fd_in;
	pid_t	last_pid;

	fd_in = STDIN_FILENO;
	last_pid = -1;
	while (cmds)
	{
		if (cmds->next && pipe(fd_pipe) == -1)
			return (perror("minishell: pipe"));
		last_pid = fork();
		if (last_pid == -1)
			return (perror("minishell: fork"));
		if (last_pid == 0)
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
	wait_all_children(sh, last_pid);
}

void	wait_all_children(t_shell *sh, pid_t last_pid)
{
	int		status;
	pid_t	pid;

	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				sh->last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				sh->last_status = 128 + WTERMSIG(status);
		}
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGQUIT)
				write(1, "Quit (core dumped)\n", 19);
			else if (WTERMSIG(status) == SIGINT)
				write(1, "\n", 1);
		}
		pid = waitpid(-1, &status, 0);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 16:53:39 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/16 17:45:14 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_directory(const char *p)
{
	struct stat	st;

	if (!p)
		return (0);
	if (stat(p, &st) == -1)
		return (0);
	return (S_ISDIR(st.st_mode));
}

void	update_parent_fds(int *fd_in, int fd_pipe[2], t_cmd *next)
{
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	if (next)
	{
		close(fd_pipe[1]);
		*fd_in = fd_pipe[0];
	}
}

char	*resolve_path(t_cmd *cmd, t_shell *sh, int i)
{
	char	*path;
	t_env	*node;

	if (ft_strchr(cmd->args[i], '/'))
		path = ft_strdup(cmd->args[i]);
	else
	{
		node = find_env_node(sh->env, "PATH");
		if (!node || !node->value || node->value[0] == '\0')
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putendl_fd(": No such file or directory", 2);
			exit(127);
		}
		path = find_path(cmd->args[i], sh->env);
	}
	if (!path || path[0] == '\0')
	{
		exe_error(cmd->args[i], path);
		exit(127);
	}
	return (path);
}

static void	handle_signal_print(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
}

void	wait_all_children(t_shell *sh, pid_t last_pid)
{
	int		status;
	pid_t	pid;

	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid < 0)
		{
			if (errno == EINTR)
				continue ;
			break ;
		}
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				sh->last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				sh->last_status = 128 + WTERMSIG(status);
		}
		handle_signal_print(status);
	}
}

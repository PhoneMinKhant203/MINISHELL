/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 21:08:30 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/09 18:56:36 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    child_exec_binary(t_cmd *cmd, t_shell *sh)
{
    char    *path;
    char    **env_arr;

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    // Handle "" directly
    if (cmd->args[0][0] == '\0')
    {
        ft_putstr_fd("minishell: : command not found\n", 2);
        exit(127);
    }

    path = find_path(cmd->args[0], sh->env);

	// 1. Check for single dot "."
    if (ft_strlen(cmd->args[0]) == 1 && cmd->args[0][0] == '.')
    {
        ft_putstr_fd("minishell: .: filename argument required\n", 2);
        exit(2); // Bash uses 2 for this
    }

	// 2. Check for double dot ".." 
    // Bash actually returns 127 "command not found" for this
    if (ft_strlen(cmd->args[0]) == 2 && ft_strncmp(cmd->args[0], "..", 2) == 0)
    {
        ft_putstr_fd("minishell: ..: command not found\n", 2);
        exit(127);
    }

    // Handle Directory (126)
    if (path && ft_strncmp(path, "IS_DIR", 7) == 0)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->args[0], 2);
        ft_putendl_fd(": Is a directory", 2);
        free(path);
        exit(126);
    }

    // Handle Command Not Found (127)
    if (!path)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->args[0], 2);
        ft_putendl_fd(": command not found", 2);
        exit(127);
    }

    env_arr = env_to_array(sh->env);
    execve(path, cmd->args, env_arr);
    
    // If execve fails, it's usually a permission issue
    perror("minishell: execve");
    exit(126);
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
	while (cmd->args[i] && cmd->args[i][0] == '\0')
		i++;
	if (!cmd->args[i] && i == 0)
		exit(0);
	if (is_builtin(cmd->args))
		exit(exe_builtin(&cmd->args[i], sh));
	else
		child_exec_binary(cmd, sh);
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

void    wait_all_children(t_shell *sh, pid_t last_pid)
{
    int     status;
    pid_t   pid;

    while ((pid = waitpid(-1, &status, 0)) > 0)
    {
        // Only update last_status if this is the last command in the pipeline
        if (pid == last_pid)
        {
            if (WIFEXITED(status))
                sh->last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                sh->last_status = 128 + WTERMSIG(status);
        }
        // Print signal messages for any process in the pipe
        if (WIFSIGNALED(status))
        {
            if (WTERMSIG(status) == SIGQUIT)
                write(1, "Quit (core dumped)\n", 19);
            else if (WTERMSIG(status) == SIGINT)
                write(1, "\n", 1);
        }
    }
}

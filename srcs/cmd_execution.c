/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_executrion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 21:08:30 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/04 21:09:56 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void start_executor(t_cmd *cmds, t_shell *sh)
{
    int     fd_pipe[2];
    int     fd_in; // This keeps track of the 'read end' for the next command
    pid_t   pid;

    fd_in = STDIN_FILENO; // The first command reads from the standard input
    while (cmds)
    {
        // 1. If there is a next command, we need a pipe
        if (cmds->next)
        {
            if (pipe(fd_pipe) == -1)
                return (perror("minishell: pipe"));
        }

        pid = fork();
        if (pid == -1)
            return (perror("minishell: fork"));
        
        if (pid == 0) // CHILD PROCESS
        {
            // Connect previous pipe's read end to STDIN
            if (fd_in != STDIN_FILENO)
            {
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            }
            // If there is a next command, connect STDOUT to this pipe's write end
            if (cmds->next)
            {
                close(fd_pipe[0]); // Child doesn't read from its own pipe
                dup2(fd_pipe[1], STDOUT_FILENO);
                close(fd_pipe[1]);
            }
            
            // Run Redirections (<, >, >>) - These have higher priority than pipes!
            if (setup_redirection(cmds) == -1)
                exit(1);

            // Execute the command
            if (is_builtin(cmds->args[0]))
                exit(exec_builtin(cmds->args, &sh->env));
            else
                child_exec_binary(cmds, sh);
        }
        else // PARENT PROCESS
        {
            // Close the read end of the PREVIOUS pipe (we are done with it)
            if (fd_in != STDIN_FILENO)
                close(fd_in);
            
            // If there's a next command, save the read end of THIS pipe for it
            if (cmds->next)
            {
                close(fd_pipe[1]); // Parent doesn't write to the pipe
                fd_in = fd_pipe[0];
            }
            cmds = cmds->next;
        }
    }
    // Final Step: Parent waits for ALL children to finish
    wait_all_children(sh);
}

void child_exec_binary(t_cmd *cmd, t_shell *sh)
{
    char    *path;
    char    **env_arr;

    path = find_path(cmd->args[0], sh->env);
    if (!path)
    {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putendl_fd(cmd->args[0], 2);
        exit(127);
    }
    env_arr = env_to_array(sh->env);
    if (execve(path, cmd->args, env_arr) == -1)
    {
        perror("minishell: execve");
        exit(126);
    }
}

void wait_all_children(t_shell *sh)
{
    int status;
    int pid;

    // wait() returns -1 when there are no more children to wait for
    while ((pid = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
            sh->last_status = WEXITSTATUS(status);
    }
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 21:08:30 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/05 14:21:03 by phonekha         ###   ########.fr       */
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
    char        *path;
    char        **env_arr;
    struct stat st;

    path = find_path(cmd->args[0], sh->env);
    if (!path)
    {
        // 1. Check if it's a directory
        if (stat(cmd->args[0], &st) == 0 && S_ISDIR(st.st_mode))
        {
            write(2, "minishell: ", 11);
            write(2, cmd->args[0], ft_strlen(cmd->args[0]));
            write(2, ": Is a directory\n", 17);
            exit(126);
        }
        // 2. Check if it's a path error (THE FIX FOR $PATH)
        if (ft_strchr(cmd->args[0], '/'))
        {
            write(2, "minishell: ", 11);
            write(2, cmd->args[0], ft_strlen(cmd->args[0]));
            write(2, ": No such file or directory\n", 28);
            exit(127);
        }
        // 3. Default command not found
        write(2, "minishell: command not found: ", 30);
        write(2, cmd->args[0], ft_strlen(cmd->args[0]));
        write(2, "\n", 1);
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


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 21:08:30 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/05 16:37:07 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void start_executor(t_cmd *cmds, t_shell *sh)
{
    int     fd_pipe[2];
    int     fd_in;
    pid_t   pid;
    int     i;

    fd_in = STDIN_FILENO;
    while (cmds)
    {
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
            if (fd_in != STDIN_FILENO)
            {
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            }
            if (cmds->next)
            {
                close(fd_pipe[0]);
                dup2(fd_pipe[1], STDOUT_FILENO);
                close(fd_pipe[1]);
            }
            if (setup_redirection(cmds) == -1)
                exit(1);

            // --- THE SHIFT FIX START ---
            i = 0;
            while (cmds->args[i] && cmds->args[i][0] == '\0')
                i++;
            
            // If everything is empty (like typing ""), just exit child quietly
            if (!cmds->args[i])
                exit(0);

            // Execute using the shifted index 'i'
            if (is_builtin(cmds->args[i]))
                exit(exec_builtin(&cmds->args[i], &sh->env));
            else
                child_exec_binary(cmds, sh);
            // --- THE SHIFT FIX END ---
        }
        else // PARENT PROCESS
        {
            if (fd_in != STDIN_FILENO)
                close(fd_in);
            if (cmds->next)
            {
                close(fd_pipe[1]);
                fd_in = fd_pipe[0];
            }
            cmds = cmds->next;
        }
    }
    wait_all_children(sh);
}


void child_exec_binary(t_cmd *cmd, t_shell *sh)
{
    char        *path;
    char        **env_arr;
    struct stat st;
    int         i;

    // 1. Skip over any empty words caused by expansion (e.g., $echo hello)
    i = 0;
    while (cmd->args[i] && cmd->args[i][0] == '\0')
        i++;

    // 2. If everything was empty (e.g., just ""), just exit the child process
    if (!cmd->args[i])
        exit(0);

    // 3. Search for the actual command (cmd->args[i])
    path = find_path(cmd->args[i], sh->env);
    
    if (!path)
    {
        // Case A: It's a directory (stat returns 0 if it exists)
        if (stat(cmd->args[i], &st) == 0 && S_ISDIR(st.st_mode))
        {
            write(2, "minishell: ", 11);
            write(2, cmd->args[i], ft_strlen(cmd->args[i]));
            write(2, ": Is a directory\n", 17);
            exit(126);
        }
        // Case B: It's a path error ($PATH or /bin/wrong)
        if (ft_strchr(cmd->args[i], '/'))
        {
            write(2, "minishell: ", 11);
            write(2, cmd->args[i], ft_strlen(cmd->args[i]));
            write(2, ": No such file or directory\n", 28);
            exit(127);
        }
        // Case C: Command Not Found (Format: command: command not found)
        write(2, cmd->args[i], ft_strlen(cmd->args[i]));
        write(2, ": command not found\n", 20);
        exit(127);
    }
    
    // 4. Prepare Environment Array
    env_arr = env_to_array(sh->env);
    
    // 5. Execute with the shifted pointer &cmd->args[i]
    if (execve(path, &cmd->args[i], env_arr) == -1)
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


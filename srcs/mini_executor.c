/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:07:58 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/06 18:32:02 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// static int	fork_and_execute(char *path, t_cmd *cmd, t_env *env_list)
// {
//     pid_t	pid;
//     int		status;
//     char    **env_arr;

//     env_arr = env_to_array(env_list);
//     pid = fork();
//     if (pid == -1)
//     {
//         free(path);
//         free2p(env_arr);
//         return (perror("minishell: fork"), 1);
//     }
//     if (pid == 0)
//     {
//         if (setup_redirection(cmd) == -1)
//             exit(1);
//         if (execve(path, cmd->args, env_arr) == -1)
//         {
//             perror("minishell: execve");
//             exit(126);
//         }
//     }
//     waitpid(pid, &status, 0);
//     free(path);
//     free2p(env_arr);
//     return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
// }

// int	exe_cmd(t_cmd *cmd, t_env *env_list)
// {
// 	char	*path;

// 	if (!cmd || !cmd->args || !cmd->args[0])
// 		return (0);
// 	path = find_path(cmd->args[0], env_list);
// 	if (!path)
// 		return (print_err(cmd->args[0], 'e'));
// 	return (fork_and_execute(path, cmd, env_list));
// }

char *find_path(char *cmd, t_env *env_list)
{
    char    **dirs;
    t_env   *node;
    char    *full;
    char    *tmp;
    struct stat st;
    int     i;

    if (!cmd || !*cmd)
        return (NULL);
    if (ft_strchr(cmd, '/'))
    {
        if (stat(cmd, &st) == 0 && S_ISREG(st.st_mode) 
				&& access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    node = find_env_node(env_list, "PATH");
    if (!node || !node->value)
        return (NULL);
    dirs = ft_split(node->value, ':');
    i = -1;
    while (dirs && dirs[++i])
    {
        tmp = ft_strjoin(dirs[i], "/");
        full = ft_strjoin(tmp, cmd);
        free(tmp);
        if (stat(full, &st) == 0 && S_ISREG(st.st_mode) 
				&& access(full, X_OK) == 0)
        {
            free2p(dirs);
            return (full);
        }
        free(full);
    }
    free2p(dirs);
    return (NULL);
}

char **env_to_array(t_env *env)
{
    char    **envp;
    int     i;
    char    *tmp;
    t_env   *curr;

    i = 0;
    curr = env;
    while (curr && ++i)
        curr = curr->next;
    envp = malloc(sizeof(char *) * (i + 1));
    if (!envp)
        return (NULL);
    i = 0;
    while (env)
    {
        tmp = ft_strjoin(env->key, "=");
        if (!tmp)
            return (free2p(envp), NULL); // Clean up if key join fails
        envp[i] = ft_strjoin(tmp, env->value);
        free(tmp);
        if (!envp[i])
            return (free2p(envp), NULL); // Clean up if value join fails
        i++;
        env = env->next;
    }
    envp[i] = NULL;
    return (envp);
}

int execute_cmds(t_cmd *cmds, t_shell *sh)
{
    int i;

    if (!cmds || !cmds->args)
        return (0);

    // SKIP EMPTY ARGUMENTS (The "$echo" fix)
    i = 0;
    while (cmds->args[i] && cmds->args[i][0] == '\0')
        i++;
    
    // If absolutely everything expanded to nothing, just stop
    if (!cmds->args[i])
        return (0);

    // 1. Parent-only Built-ins (No Pipes)
    // We check the "shifted" index [i]
    if (!cmds->next && is_builtin(cmds->args[i]))
    {
        int tmp_in = dup(STDIN_FILENO);
        int tmp_out = dup(STDOUT_FILENO);
        if (setup_redirection(cmds) == 0)
            sh->last_status = exec_builtin(&cmds->args[i], &sh->env);
        dup2(tmp_in, STDIN_FILENO);
        dup2(tmp_out, STDOUT_FILENO);
        close(tmp_in);
        close(tmp_out);
        return (sh->last_status);
    }

    // 2. Pipes and Binaries
    start_executor(cmds, sh);
    return (sh->last_status);
}

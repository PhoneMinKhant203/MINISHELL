/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:07:58 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/04 21:14:22 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int execute_cmds(t_cmd *cmds, t_shell *sh)
{
    if (!cmds)
        return (0);
    // 1. Parent-only Built-ins (No Pipes)
    if (!cmds->next && is_builtin(cmds->args[0]))
    {
        int tmp_in = dup(STDIN_FILENO);
        int tmp_out = dup(STDOUT_FILENO);
        if (setup_redirection(cmds) == 0)
            sh->last_status = exec_builtin(cmds->args, &sh->env);
        dup2(tmp_in, STDIN_FILENO);
        dup2(tmp_out, STDOUT_FILENO);
        close(tmp_in);
        close(tmp_out);
        return (sh->last_status);
    }
    // 2. Everything else (Pipes and Binaries)
    start_executor(cmds, sh);
    return (sh->last_status);
}

static int	env_list_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

char	**env_to_array(t_env *env)
{
	char	**envp;
	int		i;
	char	*tmp;

	envp = malloc(sizeof(char *) * (env_list_size(env) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		tmp = ft_strjoin(env->key, "="); 		// Join: KEY + "=" + VALUE
		envp[i] = ft_strjoin(tmp, env->value);
		free(tmp);
		i++;
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

char    *find_path(char *cmd, t_env *env_list)
{
    char    **dirs;
    t_env   *node;
    char    *full;
    char    *path_with_slash;
    struct stat st; // Buffer to hold file information
    int     i;

    if (!cmd || !*cmd)
        return (NULL);
    if (ft_strchr(cmd, '/'))
    {
        // Check if it's a file AND executable
        if (stat(cmd, &st) == 0 && S_ISREG(st.st_mode) && access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    node = find_env_node(env_list, "PATH");
    if (!node || !node->value)
        return (NULL);
    dirs = ft_split(node->value, ':');
    if (!dirs)
        return (NULL);
    i = -1;
    while (dirs[++i])
    {
        path_with_slash = ft_strjoin(dirs[i], "/");
        full = ft_strjoin(path_with_slash, cmd);
        free(path_with_slash);
        // Check stat to ensure it's not a directory
        if (stat(full, &st) == 0 && S_ISREG(st.st_mode) && access(full, X_OK) == 0)
        {
            free2p(dirs);
            return (full);
        }
        free(full);
    }
    free2p(dirs);
    return (NULL);
}

int exe_cmd(t_cmd *cmd, t_env *env_list)
{
    pid_t   pid;
    char    *path;
    char    **custom_envp;
    int     status;

    if (!cmd || !cmd->args || !cmd->args[0])
        return (0);

    path = find_path(cmd->args[0], env_list);
    if (!path)
    {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putendl_fd(cmd->args[0], 2);
        return (127);
    }

    custom_envp = env_to_array(env_list);

    pid = fork();
    if (pid == -1)
        return (perror("fork"), 1);
    if (pid == 0)
    {
        // Now you can pass 'cmd' to setup_redirection!
        if (setup_redirection(cmd) == -1)
            exit(1);
        
        if (execve(path, cmd->args, custom_envp) == -1)
        {
            perror("execve");
            exit(126);
        }
    }

    waitpid(pid, &status, 0);
    free(path);
    free2p(custom_envp); // This solves the memory leak!

    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (1);
}

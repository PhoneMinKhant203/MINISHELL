/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:07:58 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/03 17:07:59 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
    int     i;

    // 1. If it's already a path (e.g., ./minishell or /bin/ls), just return it
    if (ft_strchr(cmd, '/') && access(cmd, X_OK) == 0)
        return (ft_strdup(cmd));

    // 2. Find the PATH variable in linked list
    node = find_env_node(env_list, "PATH");
    if (!node || !node->value)
        return (NULL);

    // 3. Split the PATH into individual directories
    dirs = ft_split(node->value, ':');
    if (!dirs)
        return (NULL);

    i = 0;
    while (dirs[i])
    {
        // 4. Join: dir + "/" + cmd
        path_with_slash = ft_strjoin(dirs[i], "/");
        full = ft_strjoin(path_with_slash, cmd);
        free(path_with_slash);
        
        if (access(full, X_OK) == 0)
        {
            free2p(dirs);
            return (full);
        }
        free(full);
        i++;
    }
    free2p(dirs);
    return (NULL);
}

int exe_cmd(char **args, t_env *env_list)
{
    pid_t   pid;
    char    *path;
    char    **custom_envp;
    int     status;

    if (!args || !args[0])
        return (0);

    // 1. Find the path using our list
    path = find_path(args[0], env_list);
    if (!path)
    {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putendl_fd(args[0], 2);
        return (127);
    }

    // 2. Create the char** array for execve
    custom_envp = env_to_array(env_list);

    pid = fork();
    if (pid == -1)
        return (perror("fork"), 1);
    if (pid == 0)
    {
        // 3. Execute!
        if (execve(path, args, custom_envp) == -1)
        {
            perror("execve");
            exit(126); // Permission denied / Command cannot execute
        }
    }

    // 4. Parent waits and cleans up
    waitpid(pid, &status, 0);
    free(path);
    free2p(custom_envp);

    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (1);
}

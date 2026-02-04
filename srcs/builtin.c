/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:06:31 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/04 20:55:45 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*init_env(char **envp)
{
    t_env	*head;
    t_env	*new;
    int		i;
    char	*sep;

    head = NULL;
    i = -1;
    while (envp[++i])
    {
		sep = ft_strchr(envp[i], '=');
        if (sep)
        {
            new = malloc(sizeof(t_env));
            new->key = ft_substr(envp[i], 0, sep - envp[i]);
            new->value = ft_strdup(sep + 1);
            new->next = head;
            head = new;
        }
    }
    return (head);
}

t_env	*find_env_node(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

int	exec_builtin(char **args, t_env **env_copy)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
		return (mini_echo(args));
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (mini_pwd());
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (mini_cd(args));
	if (ft_strncmp(args[0], "env", 4) == 0)
		return (mini_env(*env_copy));
	if (ft_strncmp(args[0], "export", 7) == 0)
		return (mini_export(args, env_copy));
	if (ft_strncmp(args[0], "unset", 6) == 0)
		return (mini_unset(args, env_copy));
	if (ft_strncmp(args[0], "exit", 5) == 0)
		mini_exit(args);
	return (0);
}



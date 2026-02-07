/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 17:31:27 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/07 16:46:22 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*new;
	char	*sep;
	int		i;

	head = NULL;
	i = -1;
	while (envp && envp[++i])
	{
		sep = ft_strchr(envp[i], '=');
		if (sep)
		{
			new = malloc(sizeof(t_env));
			if (!new)
				return (NULL);
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

char	*env_get(t_env *env, char *key)
{
	t_env	*node;

	node = find_env_node(env, key);
	if (node)
		return (node->value);
	return (NULL);
}

int	env_size(t_env *env)
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
	char	*tmp;
	int		i;

	envp = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		tmp = ft_strjoin(env->key, "=");
		if (!tmp)
			return (free2p(envp), NULL);
		envp[i] = ft_strjoin(tmp, env->value);
		free1p(&tmp);
		if (!envp[i])
			return (free2p(envp), NULL);
		i++;
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 17:31:27 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/17 13:29:14 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	free_env_node(t_env *node)
{
	if (!node)
		return ;
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
}

char	**env_to_array(t_env *env)
{
	char	**envp;
	char	*tmp;
	int		i;

	envp = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!envp)
		return (NULL);
	envp[0] = NULL;
	i = 0;
	while (env)
	{
		tmp = ft_strjoin(env->key, "=");
		if (!tmp)
			return (free2p(envp), NULL);
		if (!env->value)
			envp[i] = tmp;
		else
		{
			envp[i] = ft_strjoin(tmp, env->value);
			free1p(&tmp);
		}
		if (!envp[i])
			return (free2p(envp), NULL);
		envp[i + 1] = NULL;
		i++;
		env = env->next;
	}
	return (envp);
}

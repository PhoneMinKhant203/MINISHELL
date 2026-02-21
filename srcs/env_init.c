/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 15:17:24 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/18 16:56:52 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*new_env_node(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = NULL;
	if (value)
		new->value = ft_strdup(value);
	new->next = NULL;
	if (!new->key || (value && !new->value))
	{
		free(new->key);
		free(new->value);
		free(new);
		return (NULL);
	}
	return (new);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*new;
	char	*sep;
	char	*key;

	head = NULL;
	while (envp && *envp)
	{
		sep = ft_strchr(*envp, '=');
		if (sep)
		{
			key = ft_substr(*envp, 0, sep - *envp);
			new = new_env_node(key, sep + 1);
			free(key);
			if (new)
			{
				new->next = head;
				head = new;
			}
		}
		envp++;
	}
	return (head);
}

static char	*join_env_pair(t_env *env)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(env->key, "=");
	if (!tmp)
		return (NULL);
	if (!env->value)
		return (tmp);
	res = ft_strjoin(tmp, env->value);
	free(tmp);
	return (res);
}

char	**env_to_array(t_env *env)
{
	char	**envp;
	int		i;

	envp = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!envp)
		return (NULL);
	envp[0] = NULL;
	i = 0;
	while (env)
	{
		envp[i] = join_env_pair(env);
		if (!envp[i])
		{
			while (i > 0)
				free(envp[--i]);
			free(envp);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:48:07 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/03 16:48:37 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	add_or_update_env(t_env **env, char *key, char *value)
{
	t_env	*node;
	t_env	*new;

	node = find_env_node(*env, key);
	if (node) // If it exists, update it
	{
		free(node->value);
		node->value = ft_strdup(value);
	}
	else // If not, create a new node
	{
		new = malloc(sizeof(t_env));
		new->key = ft_strdup(key);
		new->value = ft_strdup(value);
		new->next = *env;
		*env = new;
	}
}

int	mini_export(char **args, t_env **env)
{
	char	*sep;
	char	*key;
	char	*val;

	if (!args[1])
		return (mini_env(*env)); // Simple version: just print
	
	sep = ft_strchr(args[1], '=');
	if (!sep) // export KEY (with no value)
		return (0); 
        
	key = ft_substr(args[1], 0, sep - args[1]);
	val = ft_strdup(sep + 1);
	add_or_update_env(env, key, val);
	free(key);
	free(val);
	return (0);
}


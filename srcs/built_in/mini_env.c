/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:22:34 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/07 16:49:39 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	mini_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

t_env	*copy_env_list(t_env *env)
{
	t_env	*new_list;
	t_env	*new_node;

	new_list = NULL;
	while (env)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->key = ft_strdup(env->key);
		if (env->value)
			new_node->value = ft_strdup(env->value);
		else
			new_node->value = NULL;
		new_node->next = new_list;
		new_list = new_node;
		env = env->next;
	}
	return (new_list);
}

void	print_sorted_env(t_env *env)
{
	t_env	*copy;
	t_env	*tmp;

	copy = copy_env_list(env);
	sort_env_list(copy);
	tmp = copy;
	while (tmp)
	{
		printf("declare -x %s", tmp->key);
		if (tmp->value)
			printf("=\"%s\"", tmp->value);
		printf("\n");
		tmp = tmp->next;
	}
	free_env(copy);
}

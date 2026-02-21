/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:22:34 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/16 19:58:42 by wintoo           ###   ########.fr       */
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

static t_env	*copy_env_list(t_env *env)
{
	t_env	*new_list;
	t_env	*curr;
	t_env	*new;

	new_list = NULL;
	while (env)
	{
		new = new_env_node(env->key, env->value);
		if (new)
		{
			if (!new_list)
				new_list = new;
			else
				curr->next = new;
			curr = new;
		}
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

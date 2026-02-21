/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:48:07 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/16 19:57:25 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	add_or_update_env(t_env **env, char *key, char *value)
{
	t_env	*node;
	t_env	*new;

	node = find_env_node(*env, key);
	if (node)
	{
		if (value)
		{
			free(node->value);
			node->value = ft_strdup(value);
		}
	}
	else
	{
		new = new_env_node(key, value);
		if (new)
		{
			new->next = *env;
			*env = new;
		}
	}
}

void	sort_env_list(t_env *head)
{
	t_env	*i;
	t_env	*j;
	char	*tmp_key;
	char	*tmp_val;

	i = head;
	while (i)
	{
		j = i->next;
		while (j)
		{
			if (ft_strncmp(i->key, j->key, ft_strlen(i->key) + 1) > 0)
			{
				tmp_key = i->key;
				tmp_val = i->value;
				i->key = j->key;
				i->value = j->value;
				j->key = tmp_key;
				j->value = tmp_val;
			}
			j = j->next;
		}
		i = i->next;
	}
}

static int	is_valid_var_name(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	process_export_arg(char *arg, t_env **env)
{
	char	*sep;
	char	*key;

	sep = ft_strchr(arg, '=');
	if (sep)
	{
		key = ft_substr(arg, 0, sep - arg);
		add_or_update_env(env, key, sep + 1);
		free(key);
	}
	else
		add_or_update_env(env, arg, NULL);
}

int	mini_export(char **args, t_env **env)
{
	int	i;
	int	exit_status;

	if (!args[1])
		return (print_sorted_env(*env), 0);
	i = 1;
	exit_status = 0;
	while (args[i])
	{
		if (!is_valid_var_name(args[i]))
		{
			if (args[i][0] == '-' && args[i][1] != '\0')
				print_err(args[i], "export: `", "invalid option");
			else
				print_err(args[i], "export: `", "not a valid identifier");
			exit_status = 1;
		}
		else
			process_export_arg(args[i], env);
		i++;
	}
	return (exit_status);
}

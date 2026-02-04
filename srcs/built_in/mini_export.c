/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:48:07 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/04 11:53:04 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void add_or_update_env(t_env **env, char *key, char *value)
{
    t_env   *node;
    t_env   *new;

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
        new = malloc(sizeof(t_env));
        if (!new) return ;
        new->key = ft_strdup(key);
        new->value = (value) ? ft_strdup(value) : NULL;
        new->next = *env;
        *env = new;
    }
}

void sort_env_list(t_env *head)
{
    t_env   *i;
    t_env   *j;
    char    *temp_key;
    char    *temp_val;

    i = head;
    while (i)
    {
        j = i->next;
        while (j)
        {
            if (ft_strncmp(i->key, j->key, ft_strlen(i->key) + 1) > 0)
            {
                temp_key = i->key;
                temp_val = i->value;
                i->key = j->key;
                i->value = j->value;
                j->key = temp_key;
                j->value = temp_val;
            }
            j = j->next;
        }
        i = i->next;
    }
}


int is_valid_var_name(char *str)
{
    int i;

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

int mini_export(char **args, t_env **env)
{
    char    *sep;
    char    *key;
    char    *val;
    int     i;
    int     exit_status;

    if (!args[1])
	{
        print_sorted_env(*env);
        return (0);
    }
    i = 1;
    exit_status = 0;
    while (args[i])
    {
        if (!is_valid_var_name(args[i]))
        {
            fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", args[i]);
            exit_status = 1;
        }
        else
        {
            sep = ft_strchr(args[i], '=');
            if (sep) // Case: export KEY=VALUE
            {
                key = ft_substr(args[i], 0, sep - args[i]);
                val = ft_strdup(sep + 1);
                add_or_update_env(env, key, val);
                free(key);
                free(val);
            }
			else // Case: export Key (without =)
			{
				add_or_update_env(env, args[i], NULL);
			}
        }
        i++;
    }
    return (exit_status);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:44:02 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/07 18:30:17 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	mini_unset(char **args, t_env **env)
{
	t_env	*curr;
	t_env	*prev;
	int		i;

	i = 0;
	while (args[i++])
	{
		curr = *env;
		prev = NULL;
		while (curr)
		{
			if (ft_strncmp(curr->key, args[i], ft_strlen(args[i]) + 1) == 0)
			{
				if (prev)
					prev->next = curr->next;
				else
					*env = curr->next;
				free_env(curr);
				break ;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 17:31:27 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/06 17:34:32 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*env_get(t_env *env, char *key)
{
	t_env	*node;

	node = find_env_node(env, key);
	if (node)
		return (node->value);
	return (NULL);
}

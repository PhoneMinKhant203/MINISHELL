/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 13:56:07 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/18 17:33:15 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	mask_wildcard_char(char c)
{
	if (c == '*')
		return (1);
	if (c == '?')
		return (2);
	if (c == '[')
		return (3);
	return (c);
}

void	unmask_wildcards(char *s)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		if (s[i] == 1)
			s[i] = '*';
		else if (s[i] == 2)
			s[i] = '?';
		else if (s[i] == 3)
			s[i] = '[';
		i++;
	}
}

int	contains_wildcard(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == '*' || s[i] == '?' || s[i] == '[')
			return (1);
		i++;
	}
	return (0);
}

void	apply_wildcards(t_cmd *cmd)
{
	char	**new_argv;
	int		changed;
	int		i;

	changed = 0;
	new_argv = expand_wildcards_argv(cmd->args, &changed);
	if (new_argv)
	{
		free2p(cmd->args);
		cmd->args = new_argv;
	}
	i = 0;
	while (cmd->args && cmd->args[i])
		unmask_wildcards(cmd->args[i++]);
}

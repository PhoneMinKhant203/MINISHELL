/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 13:56:07 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/15 00:05:20 by phonekha         ###   ########.fr       */
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

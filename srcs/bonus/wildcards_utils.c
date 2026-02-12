/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 13:56:07 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/12 14:09:05 by wintoo           ###   ########.fr       */
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

static int	match_range(const char *pat, char c, int *consumed)
{
	int	i;
	int	neg;
	int	ok;

	i = 1;
	neg = 0;
	ok = 0;
	if (pat[i] == '!' || pat[i] == '^')
	{
		neg = 1;
		i++;
	}
	while (pat[i] && pat[i] != ']')
	{
		if (pat[i + 1] == '-' && pat[i + 2] && pat[i + 2] != ']')
		{
			if (c >= pat[i] && c <= pat[i + 2])
				ok = 1;
			i += 3;
		}
		else
		{
			if (c == pat[i])
				ok = 1;
			i++;
		}
	}
	if (pat[i] == ']')
		i++;
	*consumed = i;
	if (neg)
		return (!ok);
	return (ok);
}

static int	match_rec(const char *pat, const char *str)
{
	int	consumed;

	if (*pat == '\0')
		return (*str == '\0');
	if (*pat == '*')
	{
		while (*pat == '*')
			pat++;
		if (*pat == '\0')
			return (1);
		while (*str)
		{
			if (match_rec(pat, str))
				return (1);
			str++;
		}
		return (match_rec(pat, str));
	}
	if (*str == '\0')
		return (0);
	if (*pat == '?')
		return (match_rec(pat + 1, str + 1));
	if (*pat == '[')
	{
		if (!match_range(pat, *str, &consumed))
			return (0);
		return (match_rec(pat + consumed, str + 1));
	}
	if (*pat == *str)
		return (match_rec(pat + 1, str + 1));
	return (0);
}

int	match_pattern(const char *pat, const char *name)
{
	if (!pat || !name)
		return (0);
	if (name[0] == '.' && pat[0] != '.')
		return (0);
	return (match_rec(pat, name));
}

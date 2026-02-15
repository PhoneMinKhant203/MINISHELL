/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_math.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:05:45 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/15 00:06:05 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	match_rec(const char *pat, const char *str);

static int	check_set(const char *pat, char c, int *i)
{
	int	ok;

	ok = 0;
	while (pat[*i] && pat[*i] != ']')
	{
		if (pat[*i + 1] == '-' && pat[*i + 2] && pat[*i + 2] != ']')
		{
			if (c >= pat[*i] && c <= pat[*i + 2])
				ok = 1;
			*i += 3;
		}
		else
		{
			if (c == pat[*i])
				ok = 1;
			(*i)++;
		}
	}
	return (ok);
}

static int	match_range(const char *pat, char c, int *consumed)
{
	int	i;
	int	neg;
	int	ok;

	i = 1;
	neg = 0;
	if (pat[i] == '!' || pat[i] == '^')
	{
		neg = 1;
		i++;
	}
	ok = check_set(pat, c, &i);
	if (pat[i] == ']')
		i++;
	*consumed = i;
	if (neg)
		return (!ok);
	return (ok);
}

static int	handle_star(const char *pat, const char *str)
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

static int	match_rec(const char *pat, const char *str)
{
	int	consumed;

	if (*pat == '\0')
		return (*str == '\0');
	if (*pat == '*')
		return (handle_star(pat, str));
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

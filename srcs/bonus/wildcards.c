/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 13:55:54 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/18 17:32:55 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_matches_tail(char **m, int start)
{
	int	i;

	if (!m)
		return ;
	i = start;
	while (m[i])
	{
		free(m[i]);
		i++;
	}
	free(m);
}

static int	append_matches(char ***out, int *out_len, char **m, int *changed)
{
	int	j;

	*changed = 1;
	j = 0;
	while (m[j])
	{
		if (!push_str(out, out_len, m[j]))
			return (free_matches_tail(m, j), 0);
		j++;
	}
	free(m);
	return (1);
}

static int	process_arg(char *arg, char ***out, int *len, int *changed)
{
	char	**m;
	int		mc;
	char	*dup;

	if (!contains_wildcard(arg))
	{
		dup = ft_strdup(arg);
		if (!dup)
			return (0);
		if (!push_str(out, len, dup))
			return (free(dup), 0);
		return (1);
	}
	m = list_matches(arg, &mc);
	if (m && mc > 0)
		return (append_matches(out, len, m, changed));
	dup = ft_strdup(arg);
	if (!dup)
		return (0);
	if (!push_str(out, len, dup))
		return (free(dup), 0);
	return (1);
}

char	**expand_wildcards_argv(char **argv, int *changed)
{
	char	**out;
	int		out_len;
	int		i;

	*changed = 0;
	out = NULL;
	out_len = 0;
	i = 0;
	while (argv && argv[i])
	{
		if (!process_arg(argv[i], &out, &out_len, changed))
		{
			free2p(out);
			return (NULL);
		}
		i++;
	}
	return (out);
}

char	*expand_wildcard_redir(const char *pattern, int *ambiguous)
{
	char	**m;
	int		mc;
	char	*res;

	*ambiguous = 0;
	if (!pattern || !contains_wildcard(pattern))
	{
		if (pattern)
			return (ft_strdup(pattern));
		return (NULL);
	}
	m = list_matches(pattern, &mc);
	if (!m || mc == 0)
	{
		if (m)
			free(m);
		return (ft_strdup(pattern));
	}
	if (mc > 1)
		return (free2p(m), *ambiguous = 1, NULL);
	res = ft_strdup(m[0]);
	free2p(m);
	return (res);
}

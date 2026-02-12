/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 13:55:54 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/12 14:17:22 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <dirent.h>

static int	strcmp_safe(const char *a, const char *b)
{
	if (!a && !b)
		return (0);
	if (!a)
		return (-1);
	if (!b)
		return (1);
	return (ft_strncmp((char *)a, (char *)b, ft_strlen(a)));
}

static void	sort_strings(char **arr)
{
	int		i;
	int		j;
	char	*tmp;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (strcmp_safe(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static int	push_str(char ***arr, int *len, char *s)
{
	char	**new_arr;
	int		i;

	new_arr = malloc(sizeof(char *) * (*len + 2));
	if (!new_arr)
		return (0);
	i = 0;
	while (i < *len)
	{
		new_arr[i] = (*arr)[i];
		i++;
	}
	new_arr[*len] = s;
	new_arr[*len + 1] = NULL;
	if (*arr)
		free(*arr);
	*arr = new_arr;
	(*len)++;
	return (1);
}

static char	**list_matches(const char *pattern, int *count)
{
	DIR				*d;
	struct dirent	*ent;
	char			**matches;
	int				len;
	char			*dup;

	*count = 0;
	matches = NULL;
	len = 0;
	d = opendir(".");
	if (!d)
		return (NULL);
	ent = readdir(d);
	while (ent)
	{
		if (match_pattern(pattern, ent->d_name))
		{
			dup = ft_strdup(ent->d_name);
			if (!dup || !push_str(&matches, &len, dup))
			{
				if (dup)
					free(dup);
				closedir(d);
				if (matches)
					free2p(matches);
				return (NULL);
			}
		}
		ent = readdir(d);
	}
	closedir(d);
	if (matches)
		sort_strings(matches);
	*count = len;
	return (matches);
}

/* argv globbing: if no match => keep literal pattern (bash default) */
char	**expand_wildcards_argv(char **argv, int *changed)
{
	int		i;
	char	**out;
	int		out_len;
	char	**m;
	int		mc;
	int		j;

	*changed = 0;
	out = NULL;
	out_len = 0;
	i = 0;
	while (argv && argv[i])
	{
		if (contains_wildcard(argv[i]))
		{
			m = list_matches(argv[i], &mc);
			if (m && mc > 0)
			{
				*changed = 1;
				j = 0;
				while (m[j])
				{
					if (!push_str(&out, &out_len, m[j]))
						return (free2p(m), free2p(out), NULL);
					j++;
				}
				free(m);
			}
			else
			{
				if (m)
					free2p(m);
				if (!push_str(&out, &out_len, ft_strdup(argv[i])))
					return (free2p(out), NULL);
			}
		}
		else
		{
			if (!push_str(&out, &out_len, ft_strdup(argv[i])))
				return (free2p(out), NULL);
		}
		i++;
	}
	return (out);
}

/* redir globbing: 0 match => literal (open fails); >1 match => ambiguous */
char	*expand_wildcard_redir(const char *pattern, int *ambiguous)
{
	char	**m;
	int		mc;
	char	*res;

	*ambiguous = 0;
	if (!pattern || !contains_wildcard(pattern))
		return (pattern ? ft_strdup(pattern) : NULL);
	m = list_matches(pattern, &mc);
	if (!m || mc == 0)
	{
		if (m)
			free2p(m);
		return (ft_strdup(pattern));
	}
	if (mc > 1)
	{
		free2p(m);
		*ambiguous = 1;
		return (NULL);
	}
	res = ft_strdup(m[0]);
	free2p(m);
	return (res);
}

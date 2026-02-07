/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 13:42:47 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/07 15:24:10 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*append_str(char *s, char *to_append)
{
	char	*new;

	if (!to_append)
		return (s);
	new = ft_strjoin(s, to_append);
	free1p(&s);
	return (new);
}

char	*append_char(char *res, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (append_str(res, tmp));
}

static void	handle_special(char **rs, char *s, int *i, t_shell *sh)
{
	char	*val;

	val = NULL;
	if (s[*i] == '?')
		val = ft_itoa(sh->last_status);
	else if (s[*i] == '$')
		val = ft_strdup("4242");
	if (val)
	{
		*rs = append_str(*rs, val);
		free1p(&val);
	}
	(*i)++;
}

void	handle_dollar(char **res, char *s, int *i, t_shell *sh)
{
	int		start;
	char	*key;

	if (!s[*i + 1] || ft_isspace(s[*i + 1]) || s[*i + 1] == '\"')
		return ((void)(*res = append_char(*res, s[(*i)++])));
	(*i)++;
	if (s[*i] == '?' || s[*i] == '$')
		return (handle_special(res, s, i, sh));
	if (s[*i] == '0')
	{
		*res = append_str(*res, ft_strdup("minishell"));
		return ((void)(*i)++);
	}
	if (ft_isdigit(s[*i]))
		return ((void)(*i)++);
	start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	key = ft_substr(s, start, *i - start);
	*res = append_str(*res, env_get(sh->env, key));
	free1p(&key);
	if (!(ft_isalnum(s[*i]) || s[*i] == '_'))
		*res = append_char(*res, '$');
}

void	handle_quote(char **res, char *s, int *i, t_shell *sh)
{
	char	type;

	type = s[(*i)++];
	while (s[*i] && s[*i] != type)
	{
		if (type == '\"' && s[*i] == '$')
			handle_dollar(res, s, i, sh);
		else
			*res = append_char(*res, s[(*i)++]);
	}
	if (s[*i] == type)
		(*i)++;
}

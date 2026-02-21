/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:07:36 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/18 17:34:41 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_str(char *s, t_shell *sh)
{
	int		i;
	char	*res;

	res = ft_strdup("");
	if (!s || !res)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
			handle_quote(&res, s, &i, sh);
		else if (s[i] == '$')
			handle_dollar(&res, s, &i, sh);
		else
			res = append_char(res, s[i++]);
	}
	free1p(&s);
	return (res);
}

static int	split_expand_wd(char *str, char ***out, int *len)
{
	int		j;
	int		start;
	char	*part;

	j = 0;
	while (str[j])
	{
		while (str[j] && ft_isspace(str[j]))
			j++;
		if (!str[j])
			break ;
		start = j;
		while (str[j] && !ft_isspace(str[j]))
			j++;
		part = ft_substr(str, start, j - start);
		if (!part || !push_str(out, len, part))
		{
			if (part)
				free1p(&part);
			return (0);
		}
	}
	free1p(&str);
	return (1);
}

static int	arg_expansion(char *arg, t_shell *sh, char ***out, int *len)
{
	char	*expanded;
	int		had_quotes;

	had_quotes = has_quotes(arg);
	expanded = expand_str(ft_strdup(arg), sh);
	if (!expanded)
		return (0);
	if (had_quotes)
	{
		if (!push_str(out, len, expanded))
		{
			free1p(&expanded);
			return (0);
		}
		return (1);
	}
	return (split_expand_wd(expanded, out, len));
}

static void	process_cmd_argv(t_cmd *cmd, t_shell *sh)
{
	char	**out;
	int		i;
	int		len;

	out = NULL;
	len = 0;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		if (!arg_expansion(cmd->args[i], sh, &out, &len))
		{
			free2p(out);
			free2p(cmd->args);
			cmd->args = NULL;
			return ;
		}
		i++;
	}
	free2p(cmd->args);
	cmd->args = out;
	if (cmd->args)
		apply_wildcards(cmd);
}

void	expand_cmds(t_cmd *cmds, t_shell *sh)
{
	t_redir	*r;

	while (cmds)
	{
		if (cmds->subshell)
			expand_ast(cmds->subshell, sh);
		process_cmd_argv(cmds, sh);
		r = cmds->redirs;
		while (r)
		{
			expand_redir(r, sh);
			r = r->next;
		}
		cmds = cmds->next;
	}
}

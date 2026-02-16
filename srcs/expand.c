/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:07:36 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/16 18:35:11 by wintoo           ###   ########.fr       */
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

static void	process_cmd_argv(t_cmd *cmd, t_shell *sh)
{
	int		i;
	int		changed;
	int		had_quotes;
	char	*expanded;
	char	**out;
	int		out_len;
	int		start;
	int		j;
	char	*part;
	char	**new_argv;

	out = NULL;
	out_len = 0;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		had_quotes = has_quotes(cmd->args[i]);
		expanded = expand_str(ft_strdup(cmd->args[i]), sh);
		if (!expanded)
		{
			free2p(out);
			free2p(cmd->args);
			cmd->args = NULL;
			return ;
		}
		if (had_quotes)
		{
			if (!push_str(&out, &out_len, expanded))
			{
				free1p(&expanded);
				free2p(out);
				free2p(cmd->args);
				cmd->args = NULL;
				return ;
			}
		}
		else
		{
			j = 0;
			while (expanded[j])
			{
				while (expanded[j] && ft_isspace(expanded[j]))
					j++;
				if (!expanded[j])
					break ;
				start = j;
				while (expanded[j] && !ft_isspace(expanded[j]))
					j++;
				part = ft_substr(expanded, start, j - start);
				if (!part)
				{
					free1p(&expanded);
					free2p(out);
					free2p(cmd->args);
					cmd->args = NULL;
					return ;
				}
				if (!push_str(&out, &out_len, part))
				{
					free1p(&expanded);
					free1p(&part);
					free2p(out);
					free2p(cmd->args);
					cmd->args = NULL;
					return ;
				}
			}
			free1p(&expanded);
		}
		i++;
	}
	free2p(cmd->args);
	cmd->args = out;
	if (cmd->args)
	{
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
}

void	expand_cmds(t_cmd *cmds, t_shell *sh)
{
	t_redir	*r;

	while (cmds)
	{
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

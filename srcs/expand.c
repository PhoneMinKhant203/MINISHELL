/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:07:36 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/14 23:21:43 by phonekha         ###   ########.fr       */
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
	char	**new_argv;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		cmd->args[i] = expand_str(cmd->args[i], sh);
		i++;
	}
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

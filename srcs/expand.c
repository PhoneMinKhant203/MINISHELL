/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:07:36 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/10 16:52:53 by wintoo           ###   ########.fr       */
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

static void	expand_redir(t_redir *r, t_shell *sh)
{
	char	*org;
	char	*expanded;

	if (!r || !r->target)
		return ;
	org = ft_strdup(r->target);
	if (!org)
		return ;
	expanded = expand_str(ft_strdup(r->target), sh);
	free1p(&r->target);
	r->target = expanded;
	if (!r->target || r->target[0] == '\0')
	{
		print_err(org, NULL, "ambiguous redirect");
		sh->last_status = 1;
		free1p(&r->target);
		r->target = NULL;
	}
	free1p(&org);
}

void	expand_cmds(t_cmd *cmds, t_shell *sh)
{
	t_cmd	*curr;
	int		i;
	t_redir	*r;

	curr = cmds;
	while (curr)
	{
		i = 0;
		while (curr->args && curr->args[i])
		{
			curr->args[i] = expand_str(curr->args[i], sh);
			i++;
		}
		r = curr->redirs;
		while (r)
		{
			expand_redir(r, sh);
			r = r->next;
		}
		free1p(&curr->infile);
		free1p(&curr->outfile);
		free1p(&curr->heredoc);
		curr->append = 0;
		r = curr->redirs;
		while (r)
		{
			if (r->type == T_IN && r->target)
				curr->infile = ft_strdup(r->target);
			else if ((r->type == T_OUT || r->type == T_APPEND) && r->target)
			{
				curr->outfile = ft_strdup(r->target);
				curr->append = (r->type == T_APPEND);
			}
			else if (r->type == T_HEREDOC && r->target)
				curr->heredoc = ft_strdup(r->target);
			r = r->next;
		}
		curr = curr->next;
	}
}

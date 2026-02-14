/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:07:36 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/14 15:07:22 by wintoo           ###   ########.fr       */
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

static int  has_quotes(const char *s)
{
    int i;

    if (!s)
        return (0);
    i = 0;
    while (s[i])
    {
        if (s[i] == '\'' || s[i] == '"')
            return (1);
        i++;
	}
    return (0);
}

static int  has_space(const char *s)
{
    int i;

    if (!s)
        return (0);
    i = 0;
    while (s[i])
    {
        if (ft_isspace(s[i]))
            return (1);
        i++;
    }
    return (0);
}

static void	expand_redir(t_redir *r, t_shell *sh)
{
	char	*org;
	char	*tmp;
	int		amb;

	if (!r || !r->target)
		return ;

	/* heredoc delimiter must NOT be wildcard-expanded */
	if (r->type == T_HEREDOC)
	{
		tmp = expand_str(ft_strdup(r->target), sh);
		free1p(&r->target);
		r->target = tmp;
		if (r->target)
			unmask_wildcards(r->target);
		return ;
	}

	org = ft_strdup(r->target);
	if (!org)
		return ;

	tmp = expand_str(ft_strdup(r->target), sh);
	free1p(&r->target);
	r->target = tmp;

	amb = 0;
	tmp = expand_wildcard_redir(r->target, &amb);
	free1p(&r->target);
	r->target = tmp;
	if (!has_quotes(org) && has_space(r->target))
		amb = 1;
	if (amb || !r->target || r->target[0] == '\0')
	{
		print_err(org, NULL, "ambiguous redirect");
		sh->last_status = 1;
		free1p(&r->target);
		r->target = NULL;
		free(org);
		return ;
	}
	unmask_wildcards(r->target);
	free(org);
}

void	expand_cmds(t_cmd *cmds, t_shell *sh)
{
	t_cmd	*curr;
	int		i;
	int		changed;
	char	**new_argv;
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
		if (curr->args)
		{
			changed = 0;
			new_argv = expand_wildcards_argv(curr->args, &changed);
			if (new_argv)
			{
				free2p(curr->args);
				curr->args = new_argv;
			}
			i = 0;
			while (curr->args && curr->args[i])
			{
				unmask_wildcards(curr->args[i]);
				i++;
			}
		}
		r = curr->redirs;
		while (r)
		{
			expand_redir(r, sh);
			r = r->next;
		}
		curr = curr->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 23:22:05 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/14 23:22:43 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_ambiguous(t_redir *r, char *org, int amb, t_shell *sh)
{
	if (!has_quotes(org) && has_space(r->target))
		amb = 1;
	if (amb || !r->target || r->target[0] == '\0')
	{
		print_err(org, NULL, "ambiguous redirect");
		sh->last_status = 1;
		free1p(&r->target);
		r->target = NULL;
		free(org);
		return (1);
	}
	return (0);
}

static void	handle_heredoc_expansion(t_redir *r, t_shell *sh)
{
	char	*tmp;

	tmp = expand_str(ft_strdup(r->target), sh);
	free1p(&r->target);
	r->target = tmp;
	if (r->target)
		unmask_wildcards(r->target);
}

void	expand_redir(t_redir *r, t_shell *sh)
{
	char	*org;
	char	*tmp;
	int		amb;

	if (!r || !r->target)
		return ;
	if (r->type == T_HEREDOC)
	{
		handle_heredoc_expansion(r, sh);
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
	if (check_ambiguous(r, org, amb, sh))
		return ;
	unmask_wildcards(r->target);
	free(org);
}

int	has_quotes(const char *s)
{
	int	i;

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

int	has_space(const char *s)
{
	int	i;

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:24:08 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/10 17:15:32 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_redirs(t_redir *r)
{
	t_redir	*next;

	while (r)
	{
		next = r->next;
		free1p(&r->target);
		free(r);
		r = next;
	}
}

t_redir	*new_redir(t_tktype type, const char *target)
{
	t_redir	*r;

	r = malloc(sizeof(t_redir));
	if (!r)
		return (NULL);
	r->type = type;
	r->target = NULL;
	if (target)
	{
		r->target = ft_strdup(target);
		if (!r->target)
			return (free(r), NULL);
	}
	r->next = NULL;
	return (r);
}


void	redir_add_back(t_cmd *cmd, t_redir *node)
{
	t_redir	*cur;

	if (!cmd || !node)
		return ;
	if (!cmd->redirs)
	{
		cmd->redirs = node;
		return ;
	}
	cur = cmd->redirs;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

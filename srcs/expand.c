/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:07:36 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/06 18:19:50 by wintoo           ###   ########.fr       */
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

static void	expand_redir(char **path, t_shell *sh)
{
	char	*original;

	if (!path || !*path)
		return ;
	original = ft_strdup(*path);
	if (!original)
		return ;
	*path = expand_str(*path, sh);
	if (!*path || (*path)[0] == '\0')
		sh->last_status = print_err(original, 'r');
	free(original);
}

void	expand_cmds(t_cmd *cmds, t_shell *sh)
{
	t_cmd	*curr;
	int		i;

	curr = cmds;
	while (curr)
	{
		i = 0;
		while (curr->args && curr->args[i])
		{
			curr->args[i] = expand_str(curr->args[i], sh);
			i++;
		}
		expand_redir(&curr->infile, sh);
		expand_redir(&curr->outfile, sh);
		curr = curr->next;
	}
}

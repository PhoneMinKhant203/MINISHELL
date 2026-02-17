/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:21:40 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/17 13:48:07 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_stop(t_tktype t)
{
	return (t == T_PIPE || t == T_AND || t == T_OR);
}

int	count_args(t_token *tk)
{
	int	count;

	count = 0;
	while (tk && !is_stop(tk->type))
	{
		if (tk->type == T_WORD)
			count++;
		if (tk->type >= T_IN && tk->type <= T_HEREDOC)
			tk = tk->next;
		if (tk)
			tk = tk->next;
	}
	return (count);
}

int	fill_args(t_cmd *cmd, t_token *tk)
{
	int	i;

	i = 0;
	while (tk && !is_stop(tk->type))
	{
		if (tk->type == T_WORD)
		{
			cmd->args[i] = ft_strdup(tk->value);
			if (!cmd->args[i])
			{
				cmd->args[i] = NULL;
				return (0);
			}
			i++;
		}
		if (tk->type >= T_IN && tk->type <= T_HEREDOC)
			tk = tk->next;
		if (tk)
			tk = tk->next;
	}
	cmd->args[i] = NULL;
	return (1);
}

int	handle_redir(t_cmd *cmd, t_token *tk)
{
	t_redir	*r;

	while (tk && !is_stop(tk->type))
	{
		if ((tk->type >= T_IN && tk->type <= T_HEREDOC) && tk->next)
		{
			r = new_redir(tk->type, tk->next->value);
			if (!r)
				return (0);
			redir_add_back(cmd, r);
		}
		tk = tk->next;
	}
	return (1);
}

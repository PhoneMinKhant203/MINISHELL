/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:21:40 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/14 16:18:57 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//
// changed T_APPEND with T_HEREDOC
//

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

void	fill_args(t_cmd *cmd, t_token *tk)
{
	int	i;

	i = 0;
	while (tk && !is_stop(tk->type))
	{
		if (tk->type == T_WORD)
		{
			cmd->args[i] = ft_strdup(tk->value);
			i++;
		}
		if (tk->type >= T_IN && tk->type <= T_HEREDOC)
			tk = tk->next;
		if (tk)
			tk = tk->next;
	}
	cmd->args[i] = NULL;
}

static void	update_cmd_files(t_cmd *cmd, t_token *tk)
{
	if (tk->type == T_IN)
	{
		free1p(&cmd->infile);
		cmd->infile = ft_strdup(tk->next->value);
	}
	else if (tk->type == T_OUT || tk->type == T_APPEND)
	{
		free1p(&cmd->outfile);
		cmd->outfile = ft_strdup(tk->next->value);
		cmd->append = (tk->type == T_APPEND);
	}
	else if (tk->type == T_HEREDOC)
	{
		free1p(&cmd->heredoc);
		cmd->heredoc = ft_strdup(tk->next->value);
	}
}

void	handle_redir(t_cmd *cmd, t_token *tk)
{
	t_redir	*r;

	while (tk && !is_stop(tk->type))
	{
		if ((tk->type >= T_IN && tk->type <= T_HEREDOC) && tk->next)
		{
			r = new_redir(tk->type, tk->next->value);
			redir_add_back(cmd, r);
			update_cmd_files(cmd, tk);
		}
		tk = tk->next;
	}
}

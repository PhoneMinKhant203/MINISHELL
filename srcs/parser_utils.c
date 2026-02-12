/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:21:40 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/12 16:33:36 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//
// changed T_APPEND with T_HEREDOC
//
static int	is_stop(t_tktype t)
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

void	handle_redir(t_cmd *cmd, t_token *tk)
{
	t_redir	*r;

	while (tk && !is_stop(tk->type))
	{
		if ((tk->type == T_IN || tk->type == T_OUT
				|| tk->type == T_APPEND || tk->type == T_HEREDOC) && tk->next)
		{
			r = new_redir(tk->type, tk->next->value);
			redir_add_back(cmd, r);
		}
		if (tk->type == T_IN && tk->next)
		{
			free1p(&cmd->infile);
			cmd->infile = ft_strdup(tk->next->value);
		}
		else if ((tk->type == T_OUT || tk->type == T_APPEND) && tk->next)
		{
			free1p(&cmd->outfile);
			cmd->outfile = ft_strdup(tk->next->value);
			cmd->append = (tk->type == T_APPEND);
		}
		else if (tk->type == T_HEREDOC && tk->next)
		{
			free1p(&cmd->heredoc);
			cmd->heredoc = ft_strdup(tk->next->value);
		}
		tk = tk->next;
	}
}

t_cmd	*parse_one_cmd(t_token *tk)
{
	t_cmd	*cmd;
	int		argc;

	if (!tk || is_stop(tk->type))
		return (NULL);
	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	argc = count_args(tk);
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	if (!cmd->args)
		return (NULL);
	fill_args(cmd, tk);
	handle_redir(cmd, tk);
	return (cmd);
}

t_token	*skip_to_pipe(t_token *tk)
{
	while (tk && tk->type != T_PIPE)
		tk = tk->next;
	if (tk && tk->type == T_PIPE)
		tk = tk->next;
	return (tk);
}

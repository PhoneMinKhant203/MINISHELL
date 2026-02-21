/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 15:53:38 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/18 12:13:31 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_ast(t_node *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->type == N_PIPELINE)
		free_cmds(node->pipeline);
	free(node);
}

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->type = CMD_SIMPLE;
	cmd->args = NULL;
	cmd->subshell = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

static void	advance_to_stop(t_token **tk)
{
	while (*tk && !is_stop((*tk)->type))
		*tk = (*tk)->next;
}

static t_cmd	*parse_subshell(t_token **tk)
{
	t_cmd	*cmd;
	t_node	*sub;
	t_token	*cur;

	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	cmd->type = CMD_SUBSHELL;
	*tk = (*tk)->next;
	cur = *tk;
	sub = parse_ast(&cur);
	if (!sub || !cur || cur->type != T_RPAREN)
		return (free_ast(sub), free(cmd), NULL);
	cur = cur->next;
	cmd->subshell = sub;
	if (!handle_redir(cmd, cur))
		return (free_cmds(cmd), NULL);
	*tk = cur;
	advance_to_stop(tk);
	return (cmd);
}

t_cmd	*parse_one_cmd(t_token **tk)
{
	t_cmd	*cmd;
	int		argc;
	int		i;
	t_token	*start;

	if (!tk || !*tk || is_stop((*tk)->type))
		return (NULL);
	if ((*tk)->type == T_LPAREN)
		return (parse_subshell(tk));
	start = *tk;
	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	argc = count_args(start);
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	if (!cmd->args)
		return (free(cmd), NULL);
	i = 0;
	while (i <= argc)
		cmd->args[i++] = NULL;
	if (!fill_args(cmd, start) || !handle_redir(cmd, start))
		return (free_cmds(cmd), NULL);
	advance_to_stop(tk);
	return (cmd);
}

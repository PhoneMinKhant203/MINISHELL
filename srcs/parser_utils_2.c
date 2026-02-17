/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 15:53:38 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/17 13:51:28 by wintoo           ###   ########.fr       */
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
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_cmd	*parse_one_cmd(t_token *tk)
{
	t_cmd	*cmd;
	int		argc;
	int		i;

	if (!tk || is_stop(tk->type))
		return (NULL);
	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	argc = count_args(tk);
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	if (!cmd->args)
		return (free(cmd), NULL);
	i = 0;
	while (i <= argc)
		cmd->args[i++] = NULL;
	if (!fill_args(cmd, tk) || !handle_redir(cmd, tk))
		return (free_cmds(cmd), NULL);
	return (cmd);
}

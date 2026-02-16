/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:07:58 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/16 18:38:23 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_builtin(t_cmd *cmd, t_shell *sh, int i)
{
	int	tmp_in;
	int	tmp_out;

	tmp_in = dup(STDIN_FILENO);
	tmp_out = dup(STDOUT_FILENO);
	if (setup_redirection(cmd, sh) == 0)
		sh->last_status = exe_builtin(&cmd->args[i], sh);
	else
		sh->last_status = 1;
	dup2(tmp_in, STDIN_FILENO);
	dup2(tmp_out, STDOUT_FILENO);
	close(tmp_in);
	close(tmp_out);
	return (sh->last_status);
}

int	execute_cmds(t_cmd *cmds, t_shell *sh)
{
	if (!cmds)
		return (0);
	if (!cmds->args || !cmds->args[0])
	{
		if (setup_redirection(cmds, sh) == -1)
			return (sh->last_status = 1);
		return (sh->last_status = 0);
	}
	if (!cmds->next && is_builtin(cmds->args))
	{
		sh->last_status = handle_builtin(cmds, sh, 0);
		return (sh->last_status);
	}
	start_executor(cmds, sh);
	return (sh->last_status);
}

void	expand_ast(t_node *node, t_shell *sh)
{
	if (!node)
		return ;
	if (node->type == N_PIPELINE)
	{
		expand_cmds(node->pipeline, sh);
		return ;
	}
	expand_ast(node->left, sh);
	expand_ast(node->right, sh);
}

int	execute_ast(t_node *node, t_shell *sh)
{
	int	st;

	if (!node)
		return (0);
	if (node->type == N_PIPELINE)
		return (execute_cmds(node->pipeline, sh));
	if (node->type == N_AND)
	{
		st = execute_ast(node->left, sh);
		if (st == 0)
			st = execute_ast(node->right, sh);
		return (st);
	}
	st = execute_ast(node->left, sh);
	if (st != 0)
		st = execute_ast(node->right, sh);
	return (st);
}

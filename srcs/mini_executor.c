/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:07:58 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/10 12:21:44 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_builtin(t_cmd *cmd, t_shell *sh, int i)
{
	int	tmp_in;
	int	tmp_out;

	tmp_in = dup(STDIN_FILENO);
	tmp_out = dup(STDOUT_FILENO);
	if (setup_redirection(cmd) == 0)
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
	if (!cmds || !cmds->args || !cmds->args[0])
		return (0);
	if (!cmds->next && is_builtin(cmds->args))
	{
		sh->last_status = handle_builtin(cmds, sh, 0);
		return (sh->last_status);
	}
	start_executor(cmds, sh);
	return (sh->last_status);
}

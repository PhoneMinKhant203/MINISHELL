/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:07:58 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/08 16:28:21 by wintoo           ###   ########.fr       */
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
	int	i;

	if (!cmds || !cmds->args)
		return (0);
	i = 0;
	while (cmds->args[i] && cmds->args[i][0] == '\0')
		i++;
	if (!cmds->args[i])
		return (0);
	if (!cmds->next && is_builtin(cmds->args[i]))
		return (handle_builtin(cmds, sh, i));
	start_executor(cmds, sh);
	return (sh->last_status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:07:58 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/07 18:06:42 by wintoo           ###   ########.fr       */
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

// static int	fork_and_execute(char *path, t_cmd *cmd, t_env *env_list)
// {
//     pid_t	pid;
//     int		status;
//     char    **env_arr;

//     env_arr = env_to_array(env_list);
//     pid = fork();
//     if (pid == -1)
//     {
//         free(path);
//         free2p(env_arr);
//         return (perror("minishell: fork"), 1);
//     }
//     if (pid == 0)
//     {
//         if (setup_redirection(cmd) == -1)
//             exit(1);
//         if (execve(path, cmd->args, env_arr) == -1)
//         {
//             perror("minishell: execve");
//             exit(126);
//         }
//     }
//     waitpid(pid, &status, 0);
//     free(path);
//     free2p(env_arr);
//     return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
// }

// int	exe_cmd(t_cmd *cmd, t_env *env_list)
// {
// 	char	*path;

// 	if (!cmd || !cmd->args || !cmd->args[0])
// 		return (0);
// 	path = find_path(cmd->args[0], env_list);
// 	if (!path)
// 		return (print_err(cmd->args[0], 'e'));
// 	return (fork_and_execute(path, cmd, env_list));
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:56:25 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/02 17:31:54 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*cmd;
	char	**args;

	(void)argv;
	if (argc != 1)
		return (127); // add error later
	setup_signals();
	while (1)
	{
		cmd = readline("minishell$ ");
		if (!cmd)
		{
			printf("exit\n");
			break ;
		}
		if (cmd[0])
			add_history(cmd);
		args = tokenize(cmd);
		free(cmd);
		if (args)
		{
			exe_cmd(args, envp);
			free2p(args);
		}
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:56:25 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/04 15:20:34 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	print_tokens(t_token *t)
// {
// 	while (t)
// 	{
// 		printf("TYPE=%d VAL=[%s]\n", t->type, t->value);
// 		t = t->next;
// 	}
// }

void	print_cmds(t_cmd *cmd)
{
	int	i;

	while (cmd)
	{
		printf("CMD:\n");

		i = 0;
		while (cmd->args[i])
		{
			printf("  arg[%d]=%s\n", i, cmd->args[i]);
			i++;
		}
		if (cmd->infile)
			printf("  infile=%s\n", cmd->infile);
		if (cmd->outfile)
			printf("  outfile=%s append=%d\n",
				cmd->outfile, cmd->append);
		printf("----\n");
		cmd = cmd->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_token *token;
	t_cmd   *cmds;

	(void)argv;
	(void)envp;
	if (argc != 1)
		return (127); // add error later
	setup_signals();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (input[0])
			add_history(input);
		//
		// args = tokenize(input);
		// if (args)
		// {
		// 	exe_cmd(args, envp);
		// 	free2p(args);
		// }
		//
		token = tokenize(input);
		if (!token)
		{
			free1p(&input);
			continue ;
		}
		
		cmds = parse(token);
		if (!cmds)
		{
			free_tokens(token);
			free1p(&input);
			continue ;
		}
		print_cmds(cmds);

		free_tokens(token);
		free_cmds(cmds);
		free1p(&input);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:56:25 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/05 12:21:35 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_input(char *line, t_shell *sh)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = tokenize(line);
	if (!tokens)
		return ;
	cmds = parse(tokens);
	free_tokens(tokens); // Tokens aren't needed once we have cmds
	if (!cmds)
		return ;
	// 1. Expansion (handling $VAR) happens here
	expand_cmds(cmds, sh);	
	if (!cmds || !cmds->args || !cmds->args[0] || cmds->args[0][0] == '\0')
	{
		free_cmds(cmds);
		return ;
	}
	// 2. Execution Engine runs here
	// If it's one command and a builtin, run it in the parent.
	// If it's a binary (ls) or has pipes, run in children.
	sh->last_status = execute_cmds(cmds, sh);
	
	free_cmds(cmds);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	sh;

	(void)argv;
	if (argc != 1)
		return (1);
	sh.env = init_env(envp);
	sh.last_status = 0;
	setup_signals();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			process_input(input, &sh);
		}
		free(input);
	}
	free_env_list(sh.env);
	return (sh.last_status);
}

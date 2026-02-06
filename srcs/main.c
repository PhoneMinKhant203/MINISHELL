/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:56:25 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/06 13:39:47 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_input(char *line, t_shell *sh)
{
	t_token	*tokens;
	t_cmd	*cmds;
	int		i;

	tokens = tokenize(line);
	if (!tokens)
		return ;
	cmds = parse(tokens);
	free_tokens(tokens);
	if (!cmds)
		return ;
	expand_cmds(cmds, sh);
	i = 0;
	while (cmds->args && cmds->args[i] && cmds->args[i][0] == '\0')
		i++;
	if (!cmds->args || !cmds->args[i])
		return (free_cmds(cmds));
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

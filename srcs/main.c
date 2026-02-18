/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:56:25 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/18 12:14:34 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	shell_loop(t_shell *sh)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				ft_putendl_fd("exit", 1);
			sh->should_exit = 1;
			sh->exit_code = sh->last_status;
			break ;
		}
		sh->line_no++;
		if (g_signal == SIGINT)
		{
			sh->last_status = 130;
			g_signal = 0;
		}
		if (*input)
		{
			add_history(input);
			input = check_quotes(input, sh);
			if (input)
				process_input(input, sh);
		}
		if (input)
			free(input);
		if (sh->should_exit)
			break ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;

	(void)argv;
	if (argc != 1)
		return (1);
	sh.env = init_env(envp);
	sh.last_status = 0;
	sh.should_exit = 0;
	sh.exit_code = 0;
	sh.line_no = 0;
	update_shlvl(&sh);
	setup_signals();
	shell_loop(&sh);
	if (!sh.env)
		printf("exit\n");
	rl_clear_history();
	free_env(sh.env);
	if (sh.should_exit)
		return (sh.exit_code);
	return (sh.last_status);
}

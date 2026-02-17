/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:56:25 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/17 14:20:23 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_segment(char *line, int start, int i, t_shell *sh)
{
	char	*seg;
	char	*trim;

	seg = ft_substr(line, start, i - start);
	trim = NULL;
	if (seg)
		trim = ft_strtrim(seg, " \t\n");
	free1p(&seg);
	if (!trim || trim[0] == '\0')
	{
		ft_putendl_fd("minishell: syntax error near ';'", 2);
		sh->last_status = 2;
		free1p(&trim);
		return ;
	}
	process_an_input(trim, sh);
	free1p(&trim);
}

static void	process_last_seg(char *line, int start, int i, t_shell *sh)
{
	char	*seg;
	char	*trim;

	seg = ft_substr(line, start, i - start);
	trim = NULL;
	if (seg)
		trim = ft_strtrim(seg, " \t\n");
	free1p(&seg);
	if (trim)
	{
		if (trim[0] != '\0')
			process_an_input(trim, sh);
		free1p(&trim);
	}
}

static void	process_input(char *line, t_shell *sh)
{
	int	v[4];

	ft_memset(v, 0, sizeof(int) * 4);
	while (line && line[v[1]])
	{
		if (line[v[1]] == '\'' && !v[3])
			v[2] = !v[2];
		else if (line[v[1]] == '"' && !v[2])
			v[3] = !v[3];
		if (line[v[1]] == ';' && !v[2] && !v[3])
		{
			handle_segment(line, v[0], v[1], sh);
			v[0] = v[1] + 1;
		}
		v[1]++;
	}
	process_last_seg(line, v[0], v[1], sh);
}

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:56:25 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/14 15:04:48 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_an_input(char *line, t_shell *sh)
{
	t_token	*tokens;
	t_node	*ast;

	tokens = tokenize(line);
	if (!tokens)
		return ;
	if (!validate_syntax(tokens, sh))
		return (free_tokens(tokens));
	ast = parse_ast(tokens);
	free_tokens(tokens);
	if (!ast)
		return ;
	expand_ast(ast, sh);
	sh->last_status = execute_ast(ast, sh);
	free_ast(ast);
}

static void	process_input(char *line, t_shell *sh)
{
	int		start;
	int		i;
	int		in_s;
	int		in_d;
	char	*seg;
	char	*trim;

	start = 0;
	i = 0;
	in_s = 0;
	in_d = 0;
	while (line && line[i])
	{
		if (line[i] == '\'' && !in_d)
			in_s = !in_s;
		else if (line[i] == '"' && !in_s)
			in_d = !in_d;
		if (line[i] == ';' && !in_s && !in_d)
		{
			seg = ft_substr(line, start, i - start);
			trim = seg ? ft_strtrim(seg, " \t\n") : NULL;
			free1p(&seg);
			if (!trim || trim[0] == '\0')
			{
				ft_putendl_fd("minishell: syntax error near unexpected token ';'", 2);
				sh->last_status = 2;
				free1p(&trim);
				return ;
			}
			process_an_input(trim, sh);
			free1p(&trim);
			start = i + 1;
		}
		i++;
	}
	seg = ft_substr(line, start, i - start);
	trim = seg ? ft_strtrim(seg, " \t\n") : NULL;
	free1p(&seg);
	if (!trim || trim[0] == '\0')
	{
		if (trim)
			free1p(&trim);
		return ;
	}
	process_an_input(trim, sh);
	free1p(&trim);
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
		if (g_signal == SIGINT)
		{
			sh.last_status = 130;
			g_signal = 0;
			free1p(&input);
			continue ;
		}
		if (*input)
		{
			add_history(input);
			input = check_quotes(input, &sh);
			if (input)
				process_input(input, &sh);
		}
		free1p(&input);
	}
	return (free_env(sh.env), sh.last_status);
}

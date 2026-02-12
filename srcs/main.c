/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:56:25 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/12 16:59:10 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_input(char *line, t_shell *sh)
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
		free1p(&input);
	}
	return (free_env(sh.env), sh.last_status);
}

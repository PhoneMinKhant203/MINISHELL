/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 15:37:40 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/18 15:51:05 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_an_input(char *line, t_shell *sh)
{
	t_token	*tokens;
	t_node	*ast;
	t_token	*cur;

	tokens = tokenize(line);
	if (!tokens)
		return ;
	if (!validate_syntax(tokens, sh))
	{
		free_tokens(tokens);
		return ;
	}
	cur = tokens;
	ast = parse_ast(&cur);
	free_tokens(tokens);
	if (!ast)
		return ;
	expand_ast(ast, sh);
	sh->last_status = execute_ast(ast, sh);
	free_ast(ast);
}

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

void	handle_input(char *input, t_shell *sh)
{
	if (g_signal == SIGINT)
	{
		sh->last_status = 130;
		g_signal = 0;
	}
	if (input && *input)
	{
		add_history(input);
		input = check_quotes(input, sh);
		if (input)
			process_input(input, sh);
	}
	if (input)
		free(input);
}

void	process_input(char *line, t_shell *sh)
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

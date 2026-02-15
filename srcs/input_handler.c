/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 15:37:40 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/14 16:34:34 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_an_input(char *line, t_shell *sh)
{
	t_token	*tokens;
	t_node	*ast;

	tokens = tokenize(line);
	if (!tokens)
		return ;
	if (!validate_syntax(tokens, sh))
	{
		free_tokens(tokens);
		return ;
	}
	ast = parse_ast(tokens);
	free_tokens(tokens);
	if (!ast)
		return ;
	expand_ast(ast, sh);
	sh->last_status = execute_ast(ast, sh);
	free_ast(ast);
}

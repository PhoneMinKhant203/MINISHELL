/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 16:57:45 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/17 18:53:33 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	syntax_error(const char *tok, t_shell *sh)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (tok)
		ft_putstr_fd((char *)tok, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putendl_fd("'", 2);
	sh->last_status = 2;
}

static int	is_logic(t_tktype t)
{
	return (t == T_AND || t == T_OR);
}

static int	is_redir(t_tktype t)
{
	return (t == T_IN || t == T_OUT || t == T_APPEND || t == T_HEREDOC);
}

static int	check_token(t_token *tk, t_shell *sh)
{
	if (tk->type == T_BAD)
		return (syntax_error(tk->value, sh), 0);
	if (tk->type == T_PIPE || is_logic(tk->type))
	{
		if (!tk->next)
			return (syntax_error(NULL, sh), 0);
		if (tk->next->type == T_PIPE || is_logic(tk->next->type)
			|| tk->next->type == T_RPAREN)
			return (syntax_error(tk->next->value, sh), 0);
	}
	if (is_redir(tk->type))
	{
		if (!tk->next)
			return (syntax_error(NULL, sh), 0);
		if (tk->next->type != T_WORD)
			return (syntax_error(tk->next->value, sh), 0);
	}
	return (1);
}

int	validate_syntax(t_token *tk, t_shell *sh)
{
	int		depth;
	t_token	*prev;

	if (!tk)
		return (1);
	if (tk->type == T_BAD || tk->type == T_PIPE || is_logic(tk->type)
		|| tk->type == T_RPAREN)
	{
		syntax_error(tk->value, sh);
		return (0);
	}
	if (tk->type == T_PIPE || is_logic(tk->type))
	{
		syntax_error(tk->value, sh);
		return (0);
	}
	depth = 0;
	prev = NULL;
	while (tk)
	{
		if (tk->type == T_LPAREN)
		{
			if (prev && (prev->type == T_WORD || prev->type == T_RPAREN))
				return (syntax_error(tk->value, sh), 0);
			if (!tk->next)
				return (syntax_error(NULL, sh), 0);
			if (tk->next->type == T_RPAREN)
				return (syntax_error(tk->next->value, sh), 0);
			if (tk->next->type == T_PIPE || is_logic(tk->next->type))
				return (syntax_error(tk->next->value, sh), 0);
			depth++;
		}
		else if (tk->type == T_RPAREN)
		{
			if (depth <= 0)
				return (syntax_error(tk->value, sh), 0);
			if (!prev || prev->type == T_LPAREN || prev->type == T_PIPE
				|| is_logic(prev->type))
				return (syntax_error(tk->value, sh), 0);
			depth--;
			if (tk->next && (tk->next->type == T_WORD
					|| tk->next->type == T_LPAREN))
				return (syntax_error(tk->next->value, sh), 0);
		}
		if (!check_token(tk, sh))
			return (0);
		prev = tk;
		tk = tk->next;
	}
	if (depth != 0)
		return (syntax_error(NULL, sh), 0);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 16:57:45 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/18 18:50:59 by wintoo           ###   ########.fr       */
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

static int	check_token(t_token *tk, t_shell *sh)
{
	if (tk->type == T_BAD)
		return (syntax_error(tk->value, sh), 0);
	if (tk->type == T_PIPE || (tk->type == T_AND || tk->type == T_OR))
	{
		if (!tk->next)
			return (syntax_error(NULL, sh), 0);
		if (tk->next->type == T_PIPE || (tk->next->type == T_AND
				|| tk->next->type == T_OR) || tk->next->type == T_RPAREN)
			return (syntax_error(tk->next->value, sh), 0);
	}
	if (tk->type == T_IN || tk->type == T_OUT || tk->type == T_APPEND
		|| tk->type == T_HEREDOC)
	{
		if (!tk->next)
			return (syntax_error(NULL, sh), 0);
		if (tk->next->type != T_WORD)
			return (syntax_error(tk->next->value, sh), 0);
	}
	return (1);
}

static int	check_parenthes(t_token *tk, t_token *prev, int *d, t_shell *sh)
{
	if (tk->type == T_LPAREN)
	{
		if (prev && (prev->type == T_WORD || prev->type == T_RPAREN))
			return (syntax_error(tk->value, sh), 0);
		if (!tk->next)
			return (syntax_error(NULL, sh), 0);
		if (tk->next->type == T_PIPE || (tk->next->type == T_AND
				|| tk->next->type == T_OR) || tk->next->type == T_RPAREN)
			return (syntax_error(tk->next->value, sh), 0);
		(*d)++;
	}
	else if (tk->type == T_RPAREN)
	{
		if (*d <= 0)
			return (syntax_error(tk->value, sh), 0);
		if (!prev || prev->type == T_LPAREN || prev->type == T_PIPE
			|| (prev->type == T_AND || prev->type == T_OR))
			return (syntax_error(tk->value, sh), 0);
		(*d)--;
		if (tk->next && (tk->next->type == T_WORD
				|| tk->next->type == T_LPAREN))
			return (syntax_error(tk->next->value, sh), 0);
	}
	return (1);
}

static int	check_start_token(t_token *tk, t_shell *sh)
{
	if (!tk)
		return (1);
	if (tk->type == T_BAD || tk->type == T_PIPE || tk->type == T_RPAREN
		|| (tk->type == T_AND || tk->type == T_OR))
	{
		syntax_error(tk->value, sh);
		return (0);
	}
	return (1);
}

int	validate_syntax(t_token *tk, t_shell *sh)
{
	int		depth;
	t_token	*prev;

	if (!check_start_token(tk, sh))
		return (0);
	depth = 0;
	prev = NULL;
	while (tk)
	{
		if (!check_parenthes(tk, prev, &depth, sh))
			return (0);
		if (!check_token(tk, sh))
			return (0);
		prev = tk;
		tk = tk->next;
	}
	if (depth != 0)
		return (syntax_error(NULL, sh), 0);
	return (1);
}

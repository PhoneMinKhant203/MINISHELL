/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 16:57:45 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/12 17:12:35 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	syntax_err_tok(const char *tok, t_shell *sh)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd((char *)tok, 2);
	ft_putendl_fd("'", 2);
	sh->last_status = 2;
}

static void	syntax_err_eof(t_shell *sh)
{
	ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
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

int	validate_syntax(t_token *tk, t_shell *sh)
{
	if (!tk)
		return (1);
	if (tk->type == T_BAD)
		return (syntax_err_tok(tk->value, sh), 0);
	if (tk->type == T_PIPE || is_logic(tk->type))
		return (syntax_err_tok(tk->value, sh), 0);
	while (tk)
	{
		if (tk->type == T_BAD)
			return (syntax_err_tok(tk->value, sh), 0);
		if (tk->type == T_PIPE || is_logic(tk->type))
		{
			if (!tk->next)
				return (syntax_err_eof(sh), 0);
			if (tk->next->type == T_PIPE || is_logic(tk->next->type))
				return (syntax_err_tok(tk->next->value, sh), 0);
		}
		if (is_redir(tk->type))
		{
			if (!tk->next)
				return (syntax_err_eof(sh), 0);
			if (tk->next->type != T_WORD)
				return (syntax_err_tok(tk->next->value, sh), 0);
		}
		tk = tk->next;
	}
	return (1);
}

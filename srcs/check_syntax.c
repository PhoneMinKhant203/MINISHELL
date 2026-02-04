/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 17:19:33 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/04 17:31:14 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	is_redir(t_tktype type)
// {
// 	return (type == T_IN || type == T_OUT
// 		|| type == T_APPEND || type == T_HEREDOC);
// }

// int	check_tokens(t_token *tok)
// {
// 	if (!tok)
// 		return (0);
// 	if (tok->type == T_PIPE)
// 		return (perror("minishell"), 0);
// 	while (tok)
// 	{
// 		if (tok->type == T_PIPE)
// 		{
// 			if (!tok->next || tok->next->type == T_PIPE)
// 				return (perror("minishell"), 0);
// 		}
// 		if (is_redir(tok->type))
// 		{
// 			if (!tok->next || tok->next->type != T_WORD)
// 				return (perror("minishell"), 0);
// 		}
// 		tok = tok->next;
// 	}
// 	return (1);
// }

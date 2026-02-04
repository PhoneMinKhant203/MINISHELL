/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:48:11 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/04 19:54:06 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	skip_spaces(char *s, int *i)
{
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	if (s[*i] == '\0')
		return (0);
	return (1);
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char	*get_word(char *s, int *i)
{
	int		start;
	int		len;
	char	quote;

	len = 0;
	quote = 0;
	if (s[*i] == '\'' || s[*i] == '"')
		quote = s[(*i)++];
	start = *i;
	while (s[*i])
	{
		if (quote && s[*i] == quote)
			break ;
		if (!quote && (s[*i] == ' ' || is_operator(s[*i])))
			break ;
		(*i)++;
		len++;
	}
	if (quote && s[*i] == quote)
		(*i)++;
	return (ft_substr(s, start, len));
}

t_token	*get_token(char *s, int *i)
{
	if (s[*i] == '|')
		return ((*i)++, new_token(ft_strdup("|"), T_PIPE));
	if (s[*i] == '<')
	{
		if (s[*i + 1] == '<')
		{
			*i += 2;
			return (new_token(ft_strdup("<<"), T_HEREDOC));
		}
		return ((*i)++, new_token(ft_strdup("<"), T_IN));
	}
	if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')
		{
			*i += 2;
			return (new_token(ft_strdup(">>"), T_APPEND));
		}
		return ((*i)++, new_token(ft_strdup(">"), T_OUT));
	}
	return (new_token(get_word(s, i), T_WORD));
}

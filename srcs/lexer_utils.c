/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:48:11 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/17 13:17:44 by wintoo           ###   ########.fr       */
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

static int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>'
		|| c == '&' || c == '(' || c == ')');
}

static char	*get_word(char *s, int *i)
{
	int		start;
	char	quote;

	start = *i;
	while (s[*i] && s[*i] != ' ' && !is_operator(s[*i]))
	{
		if (s[*i] == '\'' || s[*i] == '"')
		{
			quote = s[(*i)++];
			while (s[*i] && s[*i] != quote)
				(*i)++;
			if (s[*i] == quote)
				(*i)++;
		}
		else
			(*i)++;
	}
	return (ft_substr(s, start, *i - start));
}

static t_token	*handle_op_extra(char *s, int *i)
{
	if (s[*i] == '<')
	{
		if (s[*i + 1] == '<')
			return (*i += 2, new_token(ft_strdup("<<"), T_HEREDOC));
		return ((*i)++, new_token(ft_strdup("<"), T_IN));
	}
	if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')
			return (*i += 2, new_token(ft_strdup(">>"), T_APPEND));
		return ((*i)++, new_token(ft_strdup(">"), T_OUT));
	}
	if (s[*i] == '(' || s[*i] == ')')
	{
		if (s[*i] == '(')
			return ((*i)++, new_token(ft_strdup("("), T_BAD));
		return ((*i)++, new_token(ft_strdup(")"), T_BAD));
	}
	return (NULL);
}

t_token	*get_token(char *s, int *i)
{
	if (s[*i] == '|')
	{
		if (s[*i + 1] == '|')
			return (*i += 2, new_token(ft_strdup("||"), T_OR));
		if (s[*i + 1] == '&')
			return (*i += 2, new_token(ft_strdup("|&"), T_BAD));
		return ((*i)++, new_token(ft_strdup("|"), T_PIPE));
	}
	if (s[*i] == '&')
	{
		if (s[*i + 1] == '&')
			return (*i += 2, new_token(ft_strdup("&&"), T_AND));
		return ((*i)++, new_token(ft_strdup("&"), T_BAD));
	}
	if (is_operator(s[*i]))
		return (handle_op_extra(s, i));
	return (new_token(get_word(s, i), T_WORD));
}

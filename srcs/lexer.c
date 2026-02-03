/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:14:18 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/02 14:47:41 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_words(char *s)
{
	int		i;
	int		count;
	char	quote;

	i = 0;
	count = 0;
	quote = 0;
	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		count++;
		if (s[i] && (s[i] == '\'' || s[i] == '"'))
		{
			quote = s[i++];
			while (s[i] && s[i] != quote)
				i++;
			if (s[i] && s[i] == quote)
				i++;
		}
		else
			while (s[i] && s[i] != ' ')
				i++;
	}
	return (count);
}

static int	get_wdlen(char *s, int *i)
{
	int		len;
	char	quote;

	len = 0;
	quote = 0;
	if (s[*i] == '\'' || s[*i] == '"')
		quote = s[(*i)++];
	while (s[*i])
	{
		if (quote && s[*i] == quote)
			break ;
		if (!quote && s[*i] == ' ')
			break ;
		(*i)++;
		len++;
	}
	if (quote && s[*i] == quote)
		(*i)++;
	return (len);
}

static char	*get_word(char *s, int *i)
{
	char	*word;
	int		len;
	int		start;
	int		j;

	j = 0;
	start = *i;
	if (s[*i] == '\'' || s[*i] == '"')
		start++;
	len = get_wdlen(s, i);
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	while (j < len)
	{
		word[j] = s[start + j];
		j++;
	}
	word[len] = '\0';
	return (word);
}

char	**tokenize(char *line)
{
	char	**tokens;
	int		i;
	int		j;

	tokens = malloc(sizeof(char *) * (count_words(line) + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == ' ')
		{
			i++;
			continue ;
		}
		tokens[j++] = get_word(line, &i);
	}
	tokens[j] = NULL;
	return (tokens);
}

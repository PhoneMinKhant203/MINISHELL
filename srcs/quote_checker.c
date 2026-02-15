/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:08:11 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/15 14:01:46 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_quote_state(char c, int *in_s, int *in_d)
{
	if (c == '\'' && !(*in_d))
		*in_s = !(*in_s);
	else if (c == '"' && !(*in_s))
		*in_d = !(*in_d);
}

static int	has_unclosed_quotes(const char *s)
{
	int	in_s;
	int	in_d;
	int	i;

	in_s = 0;
	in_d = 0;
	i = 0;
	while (s && s[i])
	{
		update_quote_state(s[i], &in_s, &in_d);
		i++;
	}
	return (in_s || in_d);
}

static char	*handle_unexpected_eof(char *line, t_shell *sh)
{
	ft_putendl_fd("minishell: unexpected EOF while looking for matching quote", 2);
	ft_putendl_fd("minishell: syntax error: unexpected end of file", 2);
	sh->last_status = 130;
	free(line);
	return (NULL);
}

static char	*join_with_nl(char *a, char *b)
{
	char	*tmp;
	char	*res;

	if (!a)
		return (b);
	tmp = ft_strjoin(a, "\n");
	free(a);
	if (!tmp)
	{
		free(b);
		return (NULL);
	}
	res = ft_strjoin(tmp, b);
	free(tmp);
	free(b);
	return (res);
}

char	*check_quotes(char *line, t_shell *sh)
{
	char	*more;

	while (has_unclosed_quotes(line))
	{
		more = readline("> ");
		if (g_signal == SIGINT)
		{
			sh->last_status = 130;
			g_signal = 0;
			free(more);
			free(line);
			return (NULL);
		}
		if (!more)
			return (handle_unexpected_eof(line, sh));
		line = join_with_nl(line, more);
		if (!line)
			return (NULL);
	}
	return (line);
}

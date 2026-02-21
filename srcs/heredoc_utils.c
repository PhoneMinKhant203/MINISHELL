/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:18:18 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/18 20:32:33 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_warn(const char *delimiter, int line_num)
{
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putnbr_fd(line_num, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	if (delimiter)
		ft_putstr_fd((char *)delimiter, 2);
	ft_putendl_fd("')", 2);
}

char	*strip_delim_quotes(const char *raw, int *do_expand)
{
	char	*res;
	int		i;
	int		j;

	*do_expand = 1;
	if (!raw)
		return (NULL);
	res = malloc(sizeof(char) * (ft_strlen(raw) + 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (raw[i])
	{
		if (raw[i] == '\'' || raw[i] == '\"')
			*do_expand = 0;
		else
			res[j++] = raw[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

static void	handle_special_hd(char **res, char *s, int *i, t_shell *sh)
{
	char	*val;

	val = NULL;
	if (s[*i] == '?')
		val = ft_itoa(sh->last_status);
	else if (s[*i] == '$')
		val = ft_strdup("4242");
	else if (s[*i] == '0' || ft_isdigit(s[*i]))
	{
		if (s[*i] == '0')
			val = ft_strdup("minishell");
	}
	if (val)
	{
		*res = append_str(*res, val);
		free(val);
	}
	(*i)++;
}

void	heredoc_expand_dollar(char **res, char *s, int *i, t_shell *sh)
{
	int		start;
	char	*key;
	char	*val;

	if (!s[*i + 1] || ft_isspace(s[*i + 1]))
	{
		*res = append_char(*res, s[(*i)++]);
		return ;
	}
	(*i)++;
	if (s[*i] == '?' || s[*i] == '$' || ft_isdigit(s[*i]))
		return (handle_special_hd(res, s, i, sh));
	start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	if (*i == start)
		*res = append_char(*res, '$');
	else
	{
		key = ft_substr(s, start, *i - start);
		val = env_get(sh->env, key);
		if (val)
			*res = append_str(*res, val);
		free(key);
	}
}

char	*expand_heredoc_line(char *line, t_shell *sh)
{
	int		i;
	char	*res;

	res = ft_strdup("");
	if (!line || !res)
		return (free(line), NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			heredoc_expand_dollar(&res, line, &i, sh);
		else
			res = append_char(res, line[i++]);
	}
	free(line);
	return (res);
}

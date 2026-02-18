/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:17:36 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/18 15:04:41 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	heredoc_warn(const char *delimiter, int line_num)
{
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putnbr_fd(line_num, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	if (delimiter)
		ft_putstr_fd((char *)delimiter, 2);
	ft_putendl_fd("')", 2);
}

static char	*strip_delim_quotes(const char *raw, int *do_expand)
{
	int		i;
	int		j;
	char	*delim;
	int		has_q;

	if (!raw)
		return (NULL);
	has_q = 0;
	i = 0;
	while (raw[i])
	{
		if (raw[i] == '\'' || raw[i] == '\"')
			has_q = 1;
		i++;
	}
	*do_expand = (has_q == 0);
	delim = malloc((ft_strlen(raw) + 1) * sizeof(char));
	if (!delim)
		return (NULL);
	i = 0;
	j = 0;
	while (raw[i])
	{
		if (raw[i] != '\'' && raw[i] != '\"')
			delim[j++] = raw[i];
		i++;
	}
	delim[j] = '\0';
	return (delim);
}

static void	heredoc_expand_dollar(char **res, char *s, int *i, t_shell *sh)
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
	val = NULL;
	if (s[*i] == '?')
		val = ft_itoa(sh->last_status);
	else if (s[*i] == '$')
		val = ft_strdup("4242");
	if (val)
	{
		*res = append_str(*res, val);
		free1p(&val);
		(*i)++;
		return ;
	}
	if (s[*i] == '0' || ft_isdigit(s[*i]))
	{
		if (s[*i] == '0')
			*res = append_str(*res, "minishell");
		(*i)++;
		return ;
	}
	start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	if (*i == start)
	{
		*res = append_char(*res, '$');
		return ;
	}
	key = ft_substr(s, start, *i - start);
	val = env_get(sh->env, key);
	if (val)
		*res = append_str(*res, val);
	free1p(&key);
}

static char	*expand_heredoc_line(char *line, t_shell *sh)
{
	int		i;
	char	*res;

	res = ft_strdup("");
	if (!line || !res)
	{
		free1p(&line);
		return (NULL);
	}
	i = 0;
	while (line[i])
	{
		if (line[i] == '\\' && (line[i + 1] == '$' || line[i + 1] == '\\'))
		{
			res = append_char(res, line[i + 1]);
			i += 2;
		}
		else if (line[i] == '$')
			heredoc_expand_dollar(&res, line, &i, sh);
		else
			res = append_char(res, line[i++]);
	}
	free1p(&line);
	return (res);
}

void	handle_heredoc(const char *delimiter_raw, t_shell *sh)
{
	int		fd[2];
	char	*line;
	char	*delimiter;
	int		do_expand;
	int		eof;
	int		line_len;

	if (pipe(fd) == -1)
		return (perror("minishell: pipe"));
	delimiter = strip_delim_quotes(delimiter_raw, &do_expand);
	if (!delimiter)
	{
		close(fd[0]);
		close(fd[1]);
		return ;
	}
	eof = 0;
	line_len = sh->line_no;
	while (1)
	{
		line = readline("> ");
		sh->line_no++;
		if (!line || !ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1))
		{
			if (!line)
				eof = 1;
			free(line);
			break ;
		}
		if (do_expand)
			line = expand_heredoc_line(line, sh);
		if (!line)
			break ;
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	if (eof)
		heredoc_warn(delimiter, line_len);
	free(delimiter);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:18:18 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/18 18:18:28 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*strip_delim_quotes(const char *raw, int *do_expand)
{
	int		i;
	int		j;
	char	*delim;

	if (!raw)
		return (NULL);
	delim = malloc(ft_strlen(raw) + 1);
	if (!delim)
		return (NULL);
	i = 0;
	j = 0;
	*do_expand = 1;
	while (raw[i])
	{
		if (raw[i] == '\'' || raw[i] == '\"')
		{
			*do_expand = 0;
			i++;
			continue ;
		}
		delim[j++] = raw[i++];
	}
	delim[j] = '\0';
	return (delim);
}

static void	handle_dollar_logic(char **res, char *s, int *i, t_shell *sh)
{
	char	*key;
	char	*val;
	int		start;

	(*i)++;
	if (s[*i] == '?')
		val = ft_itoa(sh->last_status);
	else if (s[*i] == '$')
		val = ft_strdup("4242");
	else
	{
		start = *i;
		while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
			(*i)++;
		key = ft_substr(s, start, *i - start);
		val = env_get(sh->env, key);
		free(key);
		if (val)
			val = ft_strdup(val);
	}
	if (val)
	{
		*res = append_str(*res, val);
		free(val);
	}
}

static char	*expand_heredoc_line(char *line, t_shell *sh)
{
	int		i;
	char	*res;

	res = ft_strdup("");
	if (!line || !res)
		return (free(line), NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && !ft_isspace(line[i + 1]))
			handle_dollar_logic(&res, line, &i, sh);
		else
			res = append_char(res, line[i++]);
	}
	free(line);
	return (res);
}

static void	heredoc_loop(int fd_write, char *delim, int expand, t_shell *sh)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		sh->line_no++;
		if (!line || !ft_strncmp(line, delim, ft_strlen(delim) + 1))
		{
			if (!line)
				heredoc_warn(delim, sh->line_no);
			free(line);
			break ;
		}
		if (expand)
			line = expand_heredoc_line(line, sh);
		if (line)
		{
			ft_putendl_fd(line, fd_write);
			free(line);
		}
	}
}

void	handle_heredoc(const char *delim_raw, t_shell *sh)
{
	int		fd[2];
	char	*delim;
	int		expand;

	if (pipe(fd) == -1)
		return (perror("minishell: pipe"));
	delim = strip_delim_quotes(delim_raw, &expand);
	if (!delim)
	{
		close(fd[0]);
		close(fd[1]);
		return ;
	}
	heredoc_loop(fd[1], delim, expand, sh);
	free(delim);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

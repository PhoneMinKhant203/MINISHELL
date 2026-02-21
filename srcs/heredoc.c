/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:17:36 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/19 12:05:48 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_delim(char *line, char *delim)
{
	if (!line)
		return (1);
	if (ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
		return (1);
	return (0);
}

static void	write_to_pipe(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

static void	heredoc_loop(int fd, char *delim, int expand, t_shell *sh)
{
	char	*line;
	int		start_line;

	start_line = sh->line_no;
	while (1)
	{
		line = readline("> ");
		sh->line_no++;
		if (is_delim(line, delim))
		{
			if (!line)
				heredoc_warn(delim, start_line);
			free(line);
			break ;
		}
		if (expand)
			line = expand_heredoc_line(line, sh);
		if (line)
			write_to_pipe(fd, line);
		else
			break ;
	}
}

void	handle_heredoc(const char *delimiter_raw, t_shell *sh)
{
	int		fd[2];
	char	*delimiter;
	int		do_expand;

	if (pipe(fd) == -1)
	{
		perror("minishell: pipe");
		return ;
	}
	delimiter = strip_delim_quotes(delimiter_raw, &do_expand);
	if (!delimiter)
	{
		close(fd[0]);
		close(fd[1]);
		return ;
	}
	heredoc_loop(fd[1], delimiter, do_expand, sh);
	free(delimiter);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

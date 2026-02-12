/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:09:32 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/12 17:23:03 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_infile(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (perror(filename), -1);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_outfile(char *filename, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd < 0)
		return (perror(filename), -1);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/*
 * Apply redirections in the same order as the user typed them.
 * bash will still create/truncate earlier output
 * files even if a later redirection fails (e.g. permission denied or missing).
 */
int	setup_redirection(t_cmd *cmd)
{
	t_redir	*r;

	if (!cmd)
		return (0);
	r = cmd->redirs;
	while (r)
	{
		if (!r->target)
			return (-1);
		if (r->type == T_IN)
		{
			if (handle_infile(r->target) == -1)
				return (-1);
		}
		else if (r->type == T_OUT)
		{
			if (handle_outfile(r->target, 0) == -1)
				return (-1);
		}
		else if (r->type == T_APPEND)
		{
			if (handle_outfile(r->target, 1) == -1)
				return (-1);
		}
		else if (r->type == T_HEREDOC)
			handle_heredoc(r->target);
		r = r->next;
	}
	return (0);
}

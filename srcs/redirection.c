/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:09:32 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/14 23:54:15 by phonekha         ###   ########.fr       */
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

static int	apply_redir(t_redir *r)
{
	if (r->type == T_IN)
		return (handle_infile(r->target));
	else if (r->type == T_OUT)
		return (handle_outfile(r->target, 0));
	else if (r->type == T_APPEND)
		return (handle_outfile(r->target, 1));
	else if (r->type == T_HEREDOC)
	{
		handle_heredoc(r->target);
		return (0);
	}
	return (0);
}

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
		if (apply_redir(r) == -1)
			return (-1);
		r = r->next;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:09:32 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/07 16:11:10 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_infile(char *filename)
{
	int	fd;

	if (!filename)
		return (0);
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

	if (!filename)
		return (0);
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

int	setup_redirection(t_cmd *cmd)
{
	if (handle_infile(cmd->infile) == -1)
		return (-1);
	if (handle_outfile(cmd->outfile, cmd->append) == -1)
		return (-1);
	return (0);
}

// int	setup_redirection(t_cmd *cmd)
// {
// 	int	fd;
// 	int	flags;

// 	if (cmd->infile)
// 	{
// 		fd = open(cmd->infile, O_RDONLY);
// 		if (fd < 0)
// 			return (perror(cmd->infile), -1);
// 		dup2(fd, STDIN_FILENO);
// 		close(fd);
// 	}
// 	if (cmd->outfile)
// 	{
// 		flags = O_WRONLY | O_CREAT;
// 		if (cmd->append)
// 			flags |= O_APPEND;
// 		else
// 			flags |= O_TRUNC;
// 		fd = open(cmd->outfile, flags, 0644);
// 		if (fd < 0)
// 			return (perror(cmd->outfile), -1);
// 		dup2(fd, STDOUT_FILENO);
// 		close(fd);
// 	}
// 	return (0);
// }

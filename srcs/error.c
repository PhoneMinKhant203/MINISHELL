/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:09:54 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/14 14:52:21 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_err(char *arg, char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
		ft_putstr_fd(cmd, 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		if (cmd && ft_strncmp(cmd, "export", 7) == 0)
			ft_putstr_fd("'", 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(msg, 2);
}

void	exe_error(char *arg, char *path)
{
	if (arg[0] == '\0')
		ft_putstr_fd("minishell: : command not found\n", 2);
	else if (ft_strncmp(arg, ".", 2) == 0)
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
	else if (ft_strncmp(arg, "..", 3) == 0)
		ft_putstr_fd("minishell: ..: command not found\n", 2);
	else if (path && ft_strncmp(path, "IS_DIR", 7) == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": Is a directory", 2);
		free(path);
		exit(126);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	exit(127);
}

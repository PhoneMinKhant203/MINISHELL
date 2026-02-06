/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:09:54 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/06 18:21:54 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_err(char *cmd, char mode)
{
	struct stat	st;

	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	if (mode == 'e')
	{
		if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		{
			ft_putendl_fd(": Is a directory", 2);
			return (126);
		}
		if (ft_strchr(cmd, '/'))
		{
			ft_putendl_fd(": No such file or directory", 2);
			return (127);
		}
		ft_putendl_fd(": command not found", 2);
	}
	else if (mode == 'r')
		return (ft_putendl_fd(": ambiguous redirect", 2), 1);
	return (127);
}

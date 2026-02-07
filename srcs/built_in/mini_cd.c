/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:10:39 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/07 18:31:54 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	mini_cd(char **args, t_env **env)
{
	char	*path;

	if (!args[1])
	{
		path = env_get(*env, "HOME");
		if (!path)
		{
			write(2, "minishell: cd: HOME not set\n", 28);
			return (1);
		}
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		write(2, "minishell: cd: ", 15);
		perror(path);
		return (1);
	}
	return (0);
}

// 1. args[0] is "cd". args[1] is the target.
// If args[1] is NULL, the user just typed "cd"
// 2. Attempt to change directory
// This will print the path + "No such file or directory" or "Permission denied"
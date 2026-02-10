/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:10:39 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/10 12:58:26 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	update_cwd_env(t_env **env)
{
	char	new_pwd[4096];
	char	old_pwd[4096];

	if (!getcwd(old_pwd, 4096))
		old_pwd[0] = '\0';
	if (old_pwd[0] != '\0')
		add_or_update_env(env, "OLDPWD", old_pwd);
	if (getcwd(new_pwd, 4096))
		add_or_update_env(env, "PWD", new_pwd);
}

int	mini_cd(char **args, t_env **env)
{
	char	*path;

	if (args[1] && args[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);
	if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
	{
		path = env_get(*env, "HOME");
		if (!path)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2), 1);
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = env_get(*env, "OLDPWD");
		if (!path)
			return (ft_putendl_fd("minishell: cd: OLDPWD not set", 2), 1);
		printf("%s\n", path);
	}
	else
		path = args[1];
	if (chdir(path) != 0)
		return (ft_putstr_fd("minishell: cd: ", 2), perror(path), 1);
	update_cwd_env(env);
	return (0);
}

// Argument and Path Logic 
// 1. args[0] is "cd". args[1] is the target.
// If args[1] is NULL, the user just typed "cd"
// 2. Save current directory as OLDPWD before changing
// We use getcwd instead of env_get to be 100% sure we have the real path
// 3. Execution
// 4. Update the Environment List
// If chdir worked, we update OLDPWD with the path we saved in step 2
// Then we get the new current path and update PWD

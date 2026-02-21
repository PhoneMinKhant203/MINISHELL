/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:10:39 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/18 20:24:59 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	update_pwd_vars(t_env **env, char old_pwd[4096], int print_pwd)
{
	char	new_pwd[4096];

	if (old_pwd[0] != '\0')
		add_or_update_env(env, "OLDPWD", old_pwd);
	if (getcwd(new_pwd, sizeof(new_pwd)))
	{
		add_or_update_env(env, "PWD", new_pwd);
		if (print_pwd)
			printf("%s\n", new_pwd);
	}
}

static char	*get_cd_path(char *arg, t_env *env, int *print_pwd)
{
	char	*path;

	*print_pwd = 0;
	if (!arg || ft_strncmp(arg, "~", 2) == 0)
	{
		path = env_get(env, "HOME");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (NULL);
		}
	}
	else if (ft_strncmp(arg, "-", 2) == 0)
	{
		path = env_get(env, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
			return (NULL);
		}
		*print_pwd = 1;
	}
	else
		path = arg;
	return (path);
}

int	mini_cd(char **args, t_env **env)
{
	char	*path;
	char	old_pwd[4096];
	int		print_pwd;

	if (args[1] && args[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		old_pwd[0] = '\0';
	path = get_cd_path(args[1], *env, &print_pwd);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	update_pwd_vars(env, old_pwd, print_pwd);
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

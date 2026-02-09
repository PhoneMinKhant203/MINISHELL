/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:10:39 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/09 19:20:13 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int mini_cd(char **args, t_env **env)
{
    char    *path;
    char    old_pwd[4096]; // Buffer to store the current path before moving
    char    new_pwd[4096]; // Buffer to store the path after moving

    // 1. Argument and Path Logic (Your existing code)
    if (args[1] && args[2])
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", 2);
        return (1);
    }
    if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
    {
        path = env_get(*env, "HOME");
        if (!path)
            return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
    }
    else if (ft_strncmp(args[1], "-", 2) == 0)
    {
        path = env_get(*env, "OLDPWD");
        if (!path)
            return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), 1);
        printf("%s\n", path);
    }
    else
        path = args[1];

    // 2. Save current directory as OLDPWD before changing
    // We use getcwd instead of env_get to be 100% sure we have the real path
    if (!getcwd(old_pwd, 4096))
        old_pwd[0] = '\0';

    // 3. Execution
    if (chdir(path) != 0)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        perror(path);
        return (1);
    }

    // 4. Update the Environment List
    // If chdir worked, we update OLDPWD with the path we saved in step 2
    if (old_pwd[0] != '\0')
        add_or_update_env(env, "OLDPWD", old_pwd);
    
    // Then we get the new current path and update PWD
    if (getcwd(new_pwd, 4096))
        add_or_update_env(env, "PWD", new_pwd);

    return (0);
}

// 1. args[0] is "cd". args[1] is the target.
// If args[1] is NULL, the user just typed "cd"
// 2. Attempt to change directory
// This will print the path + "No such file or directory" or "Permission denied"
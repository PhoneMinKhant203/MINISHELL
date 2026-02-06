/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:10:39 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/05 16:28:00 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int mini_cd(char **args, t_env **env)
{
    char    *path;

    // 1. args[0] is "cd". args[1] is the target.
    // If args[1] is NULL, the user just typed "cd"
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

    // 2. Attempt to change directory
    if (chdir(path) != 0)
    {
        write(2, "minishell: cd: ", 15);
        // This will print the path + "No such file or directory" or "Permission denied"
        perror(path); 
        return (1);
    }
    return (0);
}


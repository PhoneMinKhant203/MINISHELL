/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:56:25 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/03 17:22:36 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int main(int argc, char **argv, char **envp)
{
    char    *cmd;
    char    **args;
    t_env   *env_list;
    int     last_status;

    last_status = 0;
    (void)argv;
    if (argc != 1)
        return (127);
    
    env_list = init_env(envp);
    setup_signals();
    while (1)
    {
        cmd = readline("minishell$ ");
        if (!cmd)
        {
            printf("exit\n");
            break ;
        }
        if (cmd[0])
            add_history(cmd);
        args = tokenize(cmd);
        free(cmd);
        if (args && args[0])
        {
            // Now the types match what we fixed in the header
            if(is_builtin(args[0]))
                last_status = exec_builtin(args, &env_list);
            else
                last_status = exe_cmd(args, env_list);
            free2p(args);
        }
    }
	//still need function to free env_list
    return (last_status);
}

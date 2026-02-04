/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:40:24 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/03 23:21:24 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int is_all_digits(char *str)
{
    int i;

    i = 0;
    if (!str)
        return (0);
    if (str[i] == '-' || str[i] == '+')
        i++;
    if (!str[i])
        return (0);
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

void mini_exit(char **args)
{
    printf("exit\n");
    if (!args[1])
        exit(0); // You might want to pass g_status here later

    if (!is_all_digits(args[1]))
    {
        fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
        exit(255);
    }

    if (args[2])
    {
        fprintf(stderr, "minishell: exit: too many arguments\n");
        return ;
    }
    exit(ft_atoi(args[1]));
}


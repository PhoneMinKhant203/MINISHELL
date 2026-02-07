/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:40:24 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/07 17:52:15 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_all_digits(char *str)
{
	int	i;

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

static void	exit_error(char *arg, char *msg, int status)
{
	ft_putstr_fd("minishell: exit: ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(msg, 2);
	if (status != -1)
		exit(status);
}

void	mini_exit(char **args, t_shell *sh)
{
	ft_putendl_fd("exit", 1);
	if (!args[1])
	{
		free_env(sh->env);
		exit(sh->last_status);
	}
	if (!is_all_digits(args[1]))
	{
		free_env(sh->env);
		exit_error(args[1], "numeric argument required", 255);
	}
	if (args[2])
	{
		exit_error(NULL, "too many arguments", -1);
		return ;
	}
	free_env(sh->env);
	exit((unsigned char)ft_atoi(args[1]));
}

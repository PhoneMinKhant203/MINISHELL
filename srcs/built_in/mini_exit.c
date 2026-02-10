/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:40:24 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/10 14:23:22 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <limits.h>

static int	is_all_digits(char *str)
{
	unsigned long long	res;
	int					i;
	int					sign;

	i = 0;
	res = 0;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		res = res * 10 + (str[i++] - '0');
		if ((sign == 1 && res > (unsigned long long)LLONG_MAX)
			|| (sign == -1 && res > (unsigned long long)LLONG_MAX + 1))
			return (0);
	}
	return (1);
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
		print_err(args[1], "exit: ", "numeric argument required");
		free_env(sh->env);
		exit(2);
	}
	if (args[2])
	{
		sh->last_status = 1;
		return (print_err(NULL, "exit: ", "too many arguments"));
	}
	free_env(sh->env);
	exit((unsigned char)ft_atoi(args[1]));
}

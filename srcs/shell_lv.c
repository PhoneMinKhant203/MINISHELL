/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:00:08 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/16 16:01:13 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_signed_number(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!ft_isdigit(s[i]))
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	parse_shlvl(const char *s)
{
	long	n;
	int		sign;
	int		i;

	n = 0;
	sign = 1;
	i = 0;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	while (s[i] && ft_isdigit(s[i]) && n <= 1001)
		n = (n * 10) + (s[i++] - '0');
	return ((int)(n * sign));
}

static int	adjust_shlvl(int lvl)
{
	if (lvl < 0)
		return (0);
	if (lvl > 1000)
	{
		ft_putstr_fd("minishell: warning: shell level (", 2);
		ft_putnbr_fd(lvl, 2);
		ft_putendl_fd(") too high, resetting to 1", 2);
		return (1);
	}
	return (lvl);
}

void	update_shlvl(t_shell *sh)
{
	char	*val;
	int		lvl;
	char	*new;

	val = env_get(sh->env, "SHLVL");
	if (!is_signed_number(val))
		lvl = 1;
	else
		lvl = parse_shlvl(val) + 1;
	lvl = adjust_shlvl(lvl);
	new = ft_itoa(lvl);
	if (!new)
		return ;
	add_or_update_env(&sh->env, "SHLVL", new);
	free(new);
}

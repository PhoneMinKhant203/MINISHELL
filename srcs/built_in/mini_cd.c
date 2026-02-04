/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:10:39 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/03 20:17:40 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	mini_cd(char **args)
{
	if (!args[1])
	{
		fprintf(stderr, "minishell: cd: expected argument\n");
		return (1);
	}
	if (chdir(args[1]) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}


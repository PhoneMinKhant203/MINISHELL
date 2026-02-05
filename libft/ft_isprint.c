/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:27:39 by phonekha          #+#    #+#             */
/*   Updated: 2025/08/28 17:36:44 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(char c)
{
	return (c >= 32 && c <= 126);
}

// int main(int argc, char const *argv[])
// {
// 	int i = 0;
// 	int j = 0;
// 	while (i <= 47)
// 	{
// 		j = ft_isprint(i);
// 		printf("%d", j);
// 		i++;
// 	}
// 	return 0;
// }

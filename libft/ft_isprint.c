/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:15:26 by wintoo            #+#    #+#             */
/*   Updated: 2025/08/26 16:06:58 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isprint(int c)
{
	if ((c >= 0 && c < 32) || c >= 127)
		return (0);
	return (16384);
}

// #include <ctype.h>
// #include <stdio.h>

// int	main(void)
// {
// 	char	c;

// 	c = '\0';
// 	printf("%d\n", ft_isprint(c));
// 	printf("%d\n", isprint(c));
// }

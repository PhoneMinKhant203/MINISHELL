/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 16:01:49 by wintoo            #+#    #+#             */
/*   Updated: 2025/08/26 16:03:26 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_toupper(int c)
{
	if ((c >= 97 && c <= 122))
		return (c - 32);
	return (c);
}

// #include <ctype.h>
// #include <stdio.h>

// int	main(void)
// {
// 	char	c;

// 	c = 'A';
// 	printf("%c\n", ft_toupper(c));
// 	printf("%c\n", toupper(c));
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:45:58 by wintoo            #+#    #+#             */
/*   Updated: 2025/08/26 16:01:31 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int c)
{
	if ((c >= 65 && c <= 90))
		return (c + 32);
	return (c);
}

// #include <ctype.h>
// #include <stdio.h>

// int	main(void)
// {
// 	char	c;

// 	c = 'J';
// 	printf("%c\n", ft_tolower(c));
// 	printf("%c\n", tolower(c));
// }

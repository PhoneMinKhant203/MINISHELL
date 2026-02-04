/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:14:19 by wintoo            #+#    #+#             */
/*   Updated: 2025/08/28 19:20:37 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	char	*str;
	size_t	i;

	if (!s)
		return (NULL);
	str = (char *)s;
	i = 0;
	while (i < n)
		str[i++] = (unsigned char)c;
	return (str);
}

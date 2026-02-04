/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:25:41 by wintoo            #+#    #+#             */
/*   Updated: 2025/09/01 12:31:59 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isset(char s, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == s)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*res;
	int		start;
	int		end;
	int		i;

	if (s1 == NULL || set == NULL)
		return (NULL);
	start = 0;
	while (s1[start] && ft_isset(s1[start], set))
		start++;
	end = ft_strlen(s1) - 1;
	while (end > start && ft_isset(s1[end], set))
		end--;
	res = malloc(end - start + 2);
	if (!res)
		return (NULL);
	i = 0;
	while (s1[start] && start < end + 1)
		res[i++] = s1[start++];
	res[i] = '\0';
	return (res);
}

// int	main(void)
// {
// 	char s1[] = "telorem ipsum dolor sit amet";

// 	printf("%s\n", ft_strtrim(s1, "te"));
// }

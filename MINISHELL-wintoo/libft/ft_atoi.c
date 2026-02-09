/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:24:04 by phonekha          #+#    #+#             */
/*   Updated: 2025/08/28 17:53:52 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	long long	res;
	int			sign;

	res = 0;
	sign = 1;
	if (!str || str[0] == '\0')
		return (0);
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = (res * 10) + (*str - '0');
		if (sign == 1 && res > INT_MAX)
			return (INT_MAX);
		if (sign == -1 && res < INT_MIN)
			return (INT_MIN);
		str++;
	}
	return ((int)(sign * res));
}

// int	main(void)
// {
// 	char *str1 = "\n\n\n  -46\b9 \n5d6";
// 	char *str2 = "-54321";
// 	char *str3 = "  \t\n123";
// 	char *str4 = " -42";
// 	char *str5 = "+9876";
// 	char *str6 = "42abc";
// 	char *str7 = "abc42";
// 	char *str8 = "0";
// 	char *str9 = "     00000";
// 	char *str10 = "2147483647";
// 	char *str11 = "2147483648";
// 	char *str12 = "-2147483647";
// 	char *str13 = "-2147483649";
// 	char *str14 = "999999999999";
// 	char *str15 = "-999999999999";
// 	char *str16 = "";
// 	char *str17 = " ";
// 	char *str18 = NULL;
// 	char *str19 = "   -   123";
// 	char *str20 = "++123";
// 	char *str21 = "--123";

// 	int res1 = ft_atoi(str1);
// 	int res2 = ft_atoi(str2);
// 	int res3 = ft_atoi(str3);
// 	int res4 = ft_atoi(str4);
// 	int res5 = ft_atoi(str5);
// 	int res6 = ft_atoi(str6);
// 	int res7 = ft_atoi(str7);
// 	int res8 = ft_atoi(str8);
// 	int res9 = ft_atoi(str9);
// 	int res10 = ft_atoi(str10);
// 	int res11 = ft_atoi(str11);
// 	int res12 = ft_atoi(str12);
// 	int res13 = ft_atoi(str13);
// 	int res14 = ft_atoi(str14);
// 	int res15 = ft_atoi(str15);
// 	int res16 = ft_atoi(str16);
// 	int res17 = ft_atoi(str17);
// 	int res18 = ft_atoi(str18);
// 	int res19 = ft_atoi(str19);
// 	int res20 = ft_atoi(str20);
// 	int res21 = ft_atoi(str21);

// 	printf("%d\n", res1);
// 	printf("%d\n", res2);
// 	printf("%d\n", res3);
// 	printf("%d\n", res4);
// 	printf("%d\n", res5);
// 	printf("%d\n", res6);
// 	printf("%d\n", res7);
// 	printf("%d\n", res8);
// 	printf("%d\n", res9);
// 	printf("%d\n", res10);
// 	printf("%d\n", res11);
// 	printf("%d\n", res12);
// 	printf("%d\n", res13);
// 	printf("%d\n", res14);
// 	printf("%d\n", res15);
// 	printf("%d\n", res16);
// 	printf("%d\n", res17);
// 	printf("%d\n", res18);
// 	printf("%d\n", res19);
// 	printf("%d\n", res20);
// 	printf("%d\n", res21);
// }

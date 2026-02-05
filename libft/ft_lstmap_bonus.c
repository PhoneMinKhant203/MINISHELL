/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 09:23:59 by phonekha          #+#    #+#             */
/*   Updated: 2025/09/01 10:02:47 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*flst;
	t_list	*new;

	if (!f || !del)
		return (NULL);
	flst = NULL;
	while (lst)
	{
		new = ft_lstnew((*f)(lst->content));
		if (!new)
		{
			while (flst)
			{
				new = flst->next;
				(*del)(flst->content);
				free(flst);
				flst = new;
			}
			lst = NULL;
			return (NULL);
		}
		ft_lstadd_back(&flst, new);
		lst = lst->next;
	}
	return (flst);
}

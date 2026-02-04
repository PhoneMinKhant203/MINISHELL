/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:55:51 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/04 19:53:39 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*new_token(char *val, t_tktype type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = val;
	token->type = type;
	token->next = NULL;
	return (token);
}

t_token	*tokenize(char *line)
{
	t_token	*head;
	t_token	*tail;
	t_token	*new;
	int		i;

	i = 0;
	head = NULL;
	tail = NULL;
	while (line[i])
	{
		if (!skip_spaces(line, &i))
			break ;
		new = get_token(line, &i);
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
	}
	return (head);
}

/*
ls -l wc
.........
head: 0, tail: 0

|1|head: ls         |2| head: ls         |3| head: ls
|1|tail: 0          |2| tail: ls         |3| tail: -l
|1|tail->next: 0    |2| tail->next: -l   |3| tail->next: wc
|1|tail: ls         |2| tail: -l         |3| tail: wc
*/
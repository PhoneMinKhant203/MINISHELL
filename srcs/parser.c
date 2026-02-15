/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:19:45 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/14 16:25:42 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_node	*new_node(t_ntype type, t_node *l, t_node *r, t_cmd *p)
{
	t_node	*n;

	n = malloc(sizeof(t_node));
	if (!n)
		return (NULL);
	n->type = type;
	n->left = l;
	n->right = r;
	n->pipeline = p;
	return (n);
}

static t_cmd	*parse_pipeline(t_token **tk)
{
	t_cmd	*head;
	t_cmd	*tail;
	t_cmd	*new;

	head = NULL;
	tail = NULL;
	while (*tk && (*tk)->type != T_AND && (*tk)->type != T_OR)
	{
		new = parse_one_cmd(*tk);
		if (!new)
			return (free_cmds(head), NULL);
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
		while (*tk && (*tk)->type != T_PIPE
			&& (*tk)->type != T_AND && (*tk)->type != T_OR)
			*tk = (*tk)->next;
		if (*tk && (*tk)->type == T_PIPE)
			*tk = (*tk)->next;
	}
	return (head);
}

static t_node	*link_node(t_node *left, t_token **tok)
{
	t_node		*right;
	t_cmd		*p;
	t_tktype	op;

	op = (*tok)->type;
	*tok = (*tok)->next;
	p = parse_pipeline(tok);
	if (!p)
	{
		free_ast(left);
		return (NULL);
	}
	right = new_node(N_PIPELINE, NULL, NULL, p);
	if (!right)
	{
		free_cmds(p);
		free_ast(left);
		return (NULL);
	}
	if (op == T_AND)
		return (new_node(N_AND, left, right, NULL));
	return (new_node(N_OR, left, right, NULL));
}

t_node	*parse_ast(t_token *tok)
{
	t_node	*left;
	t_cmd	*p;

	p = parse_pipeline(&tok);
	if (!p)
		return (NULL);
	left = new_node(N_PIPELINE, NULL, NULL, p);
	if (!left)
	{
		free_cmds(p);
		return (NULL);
	}
	while (tok && (tok->type == T_AND || tok->type == T_OR))
	{
		left = link_node(left, &tok);
		if (!left)
			return (NULL);
	}
	return (left);
}

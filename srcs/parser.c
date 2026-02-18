/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:19:45 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/18 14:36:36 by wintoo           ###   ########.fr       */
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

static int	is_ast_stop(t_token *tk)
{
	return (!tk || tk->type == T_AND
		|| tk->type == T_OR || tk->type == T_RPAREN);
}

static t_cmd	*parse_pipeline(t_token **tk)
{
	t_cmd	*head;
	t_cmd	*tail;
	t_cmd	*new;

	head = NULL;
	tail = NULL;
	while (*tk && !is_ast_stop(*tk))
	{
		new = parse_one_cmd(tk);
		if (!new)
			return (free_cmds(head), NULL);
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
		if (*tk && (*tk)->type == T_PIPE)
			*tk = (*tk)->next;
		else
			break ;
	}
	return (head);
}

static t_node	*link_node(t_node *left, t_token **tok)
{
	t_node		*right;
	t_cmd		*p;
	t_tktype	op;
	t_node		*parent;

	op = (*tok)->type;
	*tok = (*tok)->next;
	p = parse_pipeline(tok);
	if (!p)
		return (free_ast(left), NULL);
	right = new_node(N_PIPELINE, NULL, NULL, p);
	if (!right)
		return (free_cmds(p), free_ast(left), NULL);
	if (op == T_AND)
		parent = new_node(N_AND, left, right, NULL);
	else
		parent = new_node(N_OR, left, right, NULL);
	if (!parent)
		return (free_ast(left), free_ast(right), NULL);
	return (parent);
}

t_node	*parse_ast(t_token **tok)
{
	t_node	*left;
	t_cmd	*p;

	if (!tok || !*tok)
		return (NULL);
	p = parse_pipeline(tok);
	if (!p)
		return (NULL);
	left = new_node(N_PIPELINE, NULL, NULL, p);
	if (!left)
	{
		free_cmds(p);
		return (NULL);
	}
	while (*tok && ((*tok)->type == T_AND || (*tok)->type == T_OR))
	{
		left = link_node(left, tok);
		if (!left)
			return (NULL);
	}
	return (left);
}

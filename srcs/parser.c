/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:19:45 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/12 16:34:18 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = NULL;
	cmd->next = NULL;
	return (cmd);
}

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
			return (NULL);
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

t_node	*parse_ast(t_token *tok)
{
	t_node	*left;
	t_node	*right;
	t_cmd	*p;
	t_tktype	op;

	p = parse_pipeline(&tok);
	if (!p)
		return (NULL);
	left = new_node(N_PIPELINE, NULL, NULL, p);
	if (!left)
		return (free_cmds(p), NULL);
	while (tok && (tok->type == T_AND || tok->type == T_OR))
	{
		op = tok->type;
		tok = tok->next;
		p = parse_pipeline(&tok);
		if (!p)
			return (free_ast(left), NULL);
		right = new_node(N_PIPELINE, NULL, NULL, p);
		if (!right)
			return (free_cmds(p), free_ast(left), NULL);
		if (op == T_AND)
			left = new_node(N_AND, left, right, NULL);
		else
			left = new_node(N_OR, left, right, NULL);
		if (!left)
			return (free_ast(right), NULL);
	}
	return (left);
}

void	free_ast(t_node *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->type == N_PIPELINE)
		free_cmds(node->pipeline);
	free(node);
}

t_cmd	*parse(t_token *token)
{
	t_cmd	*head;
	t_cmd	*tail;
	t_cmd	*new;

	head = NULL;
	tail = NULL;
	while (token)
	{
		new = parse_one_cmd(token);
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
		token = skip_to_pipe(token);
	}
	return (head);
}

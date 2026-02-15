/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 15:53:38 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/14 16:26:49 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
			return (free_cmds(head), NULL);
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
		token = skip_to_pipe(token);
	}
	return (head);
}

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

t_token	*skip_to_pipe(t_token *tk)
{
	while (tk && tk->type != T_PIPE)
		tk = tk->next;
	if (tk && tk->type == T_PIPE)
		tk = tk->next;
	return (tk);
}

t_cmd	*parse_one_cmd(t_token *tk)
{
	t_cmd	*cmd;
	int		argc;

	if (!tk || is_stop(tk->type))
		return (NULL);
	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	argc = count_args(tk);
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	fill_args(cmd, tk);
	handle_redir(cmd, tk);
	return (cmd);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 17:38:12 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/04 18:44:43 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// char	*env_get(t_env *env, char *key)
// {
// 	while (env)
// 	{
// 		if (!ft_strncmp(env->key, key, ft_strlen(key)))
// 			return (env->value);
// 		env = env->next;
// 	}
// 	return (NULL);
// }

// void	expand_cmds(t_cmd *cmds, t_shell *sh)
// {
// 	while (cmds)
// 	{
// 		expand_args(cmds->args, sh);
// 		expand_redirs(cmds, sh);
// 		cmds = cmds->next;
// 	}
// }

// void	expand_redirs(t_cmd *cmd, t_shell *sh)
// {
// 	if (cmd->infile)
// 		cmd->infile = expand_str(cmd->infile, sh);

// 	if (cmd->outfile)
// 		cmd->outfile = expand_str(cmd->outfile, sh);

// 	// heredoc handled separately
// }

// void	expand_args(char **args, t_shell *sh)
// {
// 	int	i;

// 	i = 0;
// 	while (args && args[i])
// 	{
// 		args[i] = expand_str(args[i], sh);
// 		i++;
// 	}
// }

// char	*expand_str(char *s, t_shell *sh)
// {
// 	int		i;
// 	char	*res;

// 	i = 0;
// 	res = ft_strdup("");

// 	while (s[i])
// 	{
// 		if (s[i] == '\'')
// 			res = handle_single_quote(res, s, &i);

// 		else if (s[i] == '"')
// 			res = handle_double_quote(res, s, &i, sh);

// 		else if (s[i] == '$')
// 			res = handle_dollar(res, s, &i, sh);

// 		else
// 			res = append_char(res, s[i++]);
// 	}
// 	free(s);
// 	return (res);
// }

// char	*handle_single_quote(char *res, char *s, int *i)
// {
// 	(*i)++; // skip '

// 	while (s[*i] && s[*i] != '\'')
// 		res = append_char(res, s[(*i)++]);

// 	if (s[*i] == '\'')
// 		(*i)++;

// 	return (res);
// }

// char	*handle_double_quote(char *res, char *s, int *i, t_shell *sh)
// {
// 	(*i)++;

// 	while (s[*i] && s[*i] != '"')
// 	{
// 		if (s[*i] == '$')
// 			res = handle_dollar(res, s, i, sh);
// 		else
// 			res = append_char(res, s[(*i)++]);
// 	}

// 	if (s[*i] == '"')
// 		(*i)++;

// 	return (res);
// }

// char	*handle_dollar(char *res, char *s, int *i, t_shell *sh)
// {
// 	char	*var;
// 	char	*val;
// 	int		start;

// 	(*i)++; // skip $

// 	if (s[*i] == '?')
// 	{
// 		val = ft_itoa(sh->last_status);
// 		(*i)++;
// 	}
// 	else
// 	{
// 		start = *i;
// 		while (ft_isalnum(s[*i]) || s[*i] == '_')
// 			(*i)++;

// 		var = ft_substr(s, start, *i - start);
// 		val = env_get(sh->env, var);
// 		free(var);

// 		if (!val)
// 			val = "";
// 	}

// 	res = append_str(res, val);
// 	return (res);
// }

// char	*append_char(char *s, char c)
// {
// 	char	tmp[2];

// 	tmp[0] = c;
// 	tmp[1] = 0;

// 	return (append_str(s, tmp));
// }

// char	*append_str(char *s1, char *s2)
// {
// 	char	*new;

// 	new = ft_strjoin(s1, s2);
// 	free(s1);
// 	return (new);
// }

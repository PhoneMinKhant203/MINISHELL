/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 12:31:23 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/17 13:11:40 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*check_paths(char **dirs, char *cmd)
{
	char		*full;
	char		*tmp;
	struct stat	st;
	int			i;

	i = -1;
	while (dirs && dirs[++i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		full = ft_strjoin(tmp, cmd);
		free1p(&tmp);
		if (stat(full, &st) == 0 && S_ISREG(st.st_mode)
			&& access(full, X_OK) == 0)
			return (full);
		free1p(&full);
	}
	return (NULL);
}

char	*find_path(char *cmd, t_env *env_list)
{
	t_env		*node;
	char		**dirs;
	struct stat	st;
	char		*res;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0)
		{
			if (access(cmd, X_OK) == 0)
				return (ft_strdup(cmd));
		}
		return (NULL);
	}
	node = find_env_node(env_list, "PATH");
	if (!node || !node->value)
		return (NULL);
	dirs = ft_split(node->value, ':');
	res = check_paths(dirs, cmd);
	free2p(dirs);
	return (res);
}

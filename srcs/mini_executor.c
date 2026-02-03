/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:07:58 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/02 18:20:25 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_path(char *cmd)
{
	char	*path;
	char	**dirs;
	char	*full;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));

	path = getenv("PATH");
	if (!path)
		return (NULL);

	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);

	i = 0;
	while (dirs[i])
	{
		full = mod_strjoin(dirs[i], cmd);
		if (access(full, X_OK) == 0)
		{
			free2p(dirs);
			return (full);
		}
		free(full);
		i++;
	}

	free2p(dirs);
	return (NULL);
}

int	exe_cmd(char **args, char **envp)
{
	pid_t	pid;
	char	*path;
	int		status;

	if (!args || !args[0])
		return (0);
	path = find_path(args[0]);
	if (!path)
		return (perror(args[0]), 127);
	pid = fork();
	if (pid == 0)
	{
		execve(path, args, envp);
		perror("execve");
		exit(1);
	}
	else if (pid < 0)
		return (perror("fork"), 1);
	waitpid(pid, &status, 0);
	free(path);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

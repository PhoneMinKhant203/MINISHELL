/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 10:29:50 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/03 09:10:31 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_null(char **ptr)
{
	if (*ptr != NULL)
	{
		free(*ptr);
		ptr = NULL;
	}
}

char	*join_line(char **buffer, int nl_pos)
{
	char	*res;
	char	*temp;

	temp = NULL;
	if (nl_pos <= 0)
	{
		if (**buffer == '\0')
		{
			free(*buffer);
			*buffer = NULL;
			return (NULL);
		}
		res = *buffer;
		*buffer = NULL;
		return (res);
	}
	temp = ft_substr(*buffer, nl_pos, BUFFER_SIZE);
	res = *buffer;
	res[nl_pos] = 0;
	*buffer = temp;
	return (res);
}

char	*read_line(int fd, char **buffer, char *read_res)
{
	ssize_t	byte_read;
	char	*temp;
	char	*nl;

	nl = ft_strchr(*buffer, '\n');
	temp = NULL;
	byte_read = 0;
	while (nl == NULL)
	{
		byte_read = read(fd, read_res, BUFFER_SIZE);
		if (byte_read <= 0)
			return (join_line(buffer, byte_read));
		read_res[byte_read] = 0;
		temp = ft_strjoin(*buffer, read_res);
		free_null(buffer);
		*buffer = temp;
		nl = ft_strchr(*buffer, '\n');
	}
	return (join_line(buffer, nl - *buffer + 1));
}

char	*get_next_line(int fd)
{
	static char	*buffer[MAX_FD + 1];
	char		*read_res;
	char		*res;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > MAX_FD)
		return (NULL);
	read_res = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (read_res == NULL)
		return (NULL);
	if (!buffer[fd])
		buffer[fd] = ft_strdup("");
	res = read_line(fd, &buffer[fd], read_res);
	free_null(&read_res);
	return (res);
}

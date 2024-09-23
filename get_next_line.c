/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 22:16:59 by mmravec           #+#    #+#             */
/*   Updated: 2024/09/23 23:20:52 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include <stdio.h>

static char	*append_buffer(char *main_buffer, char *read_buffer);

static char	*extract_line(char *buffer)
{
	char	*temp;
	size_t	line_length;
	char	*line_end;
	size_t	i;

	line_end = ft_strchr(buffer, '\n');
	if (line_end == NULL)
		line_length = ft_strlen(buffer);
	else
		line_length = line_end - buffer + 1;
	temp = (char *) ft_calloc(line_length + 1, sizeof(char));
	i = 0;
	while (i < line_length)
	{
		temp[i] = buffer[i];
		i++;
	}
	temp[i] = '\0';
	if (temp[0] == '\0')
	{
		free(temp);
		return (NULL);
	}
	return (temp);
}

static char	*obtain_remaining(char *buffer)
{
	char	*temp;
	char	*line_end;

	line_end = ft_strchr(buffer, '\n');
	if (line_end == NULL)
	{
		free(buffer);
		return (NULL);
	}
	temp = (char *) ft_calloc(ft_strlen(line_end + 1) + 1, sizeof(char));
	if (!temp)
		return (free(buffer), NULL);
	line_end++;
	ft_strlcpy(temp, line_end, ft_strlen(line_end) + 1);
	free(buffer);
	return (temp);
}

static char	*read_from_file(char *main_buffer, int fd)
{
	char	*local_buffer;
	int		bytes_read;

	local_buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!local_buffer)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, local_buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(local_buffer), NULL);
		local_buffer[bytes_read] = '\0';
		main_buffer = append_buffer(main_buffer, local_buffer);
		if (ft_strchr(main_buffer, '\n'))
			break ;
	}
	free (local_buffer);
	return (main_buffer);
}

static char	*append_buffer(char *main_buffer, char *read_buffer)
{
	size_t	main_len;
	size_t	read_len;
	char	*temp;

	main_len = ft_strlen(main_buffer);
	read_len = ft_strlen(read_buffer);
	temp = (char *)malloc(main_len + read_len + 1);
	if (!temp)
	{
		free(main_buffer);
		return (NULL);
	}
	ft_strlcpy(temp, main_buffer, main_len + 1);
	ft_strlcpy(temp + main_len, read_buffer, read_len + 1);
	free(main_buffer);
	return (temp);
}

char	*get_next_line(int fd)
{
	static char	*main_buffer;
	char		*line;

	if (fd < 0 || read(fd, NULL, 0) < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!main_buffer)
		main_buffer = ft_calloc(1, sizeof (char));
	main_buffer = read_from_file(main_buffer, fd);
	if (!main_buffer)
		return (NULL);
	line = extract_line(main_buffer);
	main_buffer = obtain_remaining(main_buffer);
	return (line);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 13:35:45 by mobullad          #+#    #+#             */
/*   Updated: 2025/04/20 03:04:02 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_get_line(char *buff)
{
	size_t	i;
	char	*line;

	i = 0;
	while (buff[i] && buff[i] != '\n')
		i++;
	line = ft_substr(buff, 0, i + (buff[i] == '\n'));
	return (line);
}

void	clear_reserv(char *reserv)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (reserv[i] && reserv[i] != '\n')
		i++;
	if (reserv[i] == '\n')
		i++;
	len = ft_strlen(reserv) - i;
	ft_memmove(reserv, reserv + i, len);
	reserv[len] = '\0';
}

char	*join_lines(char *line, char *temp)
{
	char	*new_line;

	if (!line)
		line = ft_strdup("");
	new_line = ft_strjoin(line, temp);
	free(line);
	free(temp);
	return (new_line);
}

char	*get_next_line_exec(int fd, char *line, char *reserv)
{
	ssize_t		nbytes;

	nbytes = 1;
	while (nbytes > 0)
	{
		if (!reserv[0])
		{
			nbytes = read(fd, reserv, BUFFER_SIZE);
			if (nbytes <= 0)
			{
				if (nbytes == 0)
					break ;
				return (free(line), NULL);
			}
			reserv[nbytes] = '\0';
		}
		line = join_lines(line, ft_get_line(reserv));
		if (ft_strchr(reserv, '\n'))
		{
			clear_reserv(reserv);
			break ;
		}
		reserv[0] = '\0';
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	reserv[MAX_FD][BUFFER_SIZE + 1];
	char		*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0 || fd > MAX_FD)
		return (NULL);
	line = get_next_line_exec(fd, line, reserv[fd]);
	return (line);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc_input.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 00:00:00 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_signal_interruption(char *line)
{
	if (g_signal_status == SIGINT)
	{
		our_free(line);
		return (1);
	}
	return (0);
}

int	handle_read_result(int bytes_read, int i, char *line)
{
	if (bytes_read <= 0)
	{
		if (i == 0)
		{
			our_free(line);
			return (-1);
		}
		return (1);
	}
	return (0);
}

int	process_char_input(char *line, int *i)
{
	char	buffer[1];
	int		bytes_read;
	int		result;

	if (handle_signal_interruption(line))
		return (-1);
	bytes_read = read(STDIN_FILENO, buffer, 1);
	if (handle_signal_interruption(line))
		return (-1);
	result = handle_read_result(bytes_read, *i, line);
	if (result == -1)
		return (-1);
	if (result == 1)
		return (1);
	if (buffer[0] == '\n')
		return (1);
	line[*i] = buffer[0];
	(*i)++;
	return (0);
}

char	*read_line_with_signal_check(void)
{
	char	*line;
	int		i;
	int		result;

	line = our_malloc(1024);
	if (!line)
		return (NULL);
	i = 0;
	write(STDOUT_FILENO, "> ", 2);
	while (i < 1023)
	{
		result = process_char_input(line, &i);
		if (result == -1)
			return (NULL);
		if (result == 1)
			break ;
	}
	line[i] = '\0';
	return (line);
}

char	*read_heredoc_line_input(void)
{
	char	*line;
	size_t	len;

	len = 0;
	if (isatty(STDIN_FILENO))
	{
		line = read_line_with_signal_check();
		if (g_signal_status == SIGINT)
		{
			if (line)
				our_free(line);
			return (NULL);
		}
	}
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (line)
		{
			len = ft_strlen(line);
			if (len > 0 && line[len - 1] == '\n')
				line[len - 1] = '\0';
		}
	}
	return (line);
}

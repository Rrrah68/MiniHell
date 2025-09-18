/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/15 00:00:00 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delimiter(const char *line, const char *delimiter)
{
	size_t	len;

	if (!line || !delimiter)
		return (0);
	len = ft_strlen(delimiter);
	if (ft_strncmp(line, delimiter, len) == 0 && line[len] == '\0')
		return (1);
	else
		return (0);
}

void	write_heredoc_line(int fd, const char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

int	handle_heredoc_with_content(const char *content)
{
	int		pipefd[2];
	size_t	len;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (content)
	{
		len = ft_strlen(content);
		write(pipefd[1], content, len);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

static char	*read_line_with_signal_check(void)
{
	char	*line;
	char	buffer[1];
	int		bytes_read;
	int		i;

	line = malloc(1024);
	if (!line)
		return (NULL);
	i = 0;
	write(STDOUT_FILENO, "> ", 2);
	
	while (i < 1023)
	{
		// Vérifier le signal avant chaque lecture
		if (g_signal_status == SIGINT)
		{
			free(line);
			return (NULL);
		}
		
		bytes_read = read(STDIN_FILENO, buffer, 1);
		
		// Vérifier le signal immédiatement après la lecture
		if (g_signal_status == SIGINT)
		{
			free(line);
			return (NULL);
		}
		
		if (bytes_read <= 0)
		{
			// EOF détecté - retourner la ligne vide si on n'a rien lu
			if (i == 0)
			{
				free(line);
				return (NULL);
			}
			break;
		}
		if (buffer[0] == '\n')
			break;
		line[i] = buffer[0];
		i++;
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
		// Pour les heredocs, on utilise une lecture caractère par caractère
		// pour pouvoir vérifier les signaux entre chaque caractère
		line = read_line_with_signal_check();
		// Vérifier si un signal SIGINT a été reçu
		if (g_signal_status == SIGINT)
		{
			if (line)
				free(line);
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

void	handle_heredoc_eof_warning(const char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document delimited by ",
		STDERR_FILENO);
	ft_putstr_fd("end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd((char *)delimiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

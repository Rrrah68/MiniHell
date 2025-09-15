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

char	*read_heredoc_line_input(void)
{
	char	*line;
	size_t	len;

	len = 0;
	if (isatty(STDIN_FILENO))
		line = readline("> ");
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

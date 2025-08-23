/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:31:08 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/15 18:34:22 by mobullad         ###   ########.fr       */
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

int	handle_heredoc(const char *delimiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || is_delimiter(line, delimiter))
			break ;
		write_heredoc_line(pipefd[1], line);
		free(line);
	}
	free(line);
	close(pipefd[1]);
	return (pipefd[0]);
}

int	setup_heredoc(t_cmd *cmd)
{
	if (!cmd->heredoc_limiter)
		return (0);
	cmd->heredoc_fd = handle_heredoc(cmd->heredoc_limiter);
	if (cmd->heredoc_fd == -1)
		return (-1);
	if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 heredoc");
		close(cmd->heredoc_fd);
		return (-1);
	}
	close(cmd->heredoc_fd);
	return (0);
}

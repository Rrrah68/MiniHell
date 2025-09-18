/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 21:08:39 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_heredoc_line(const char *line, t_data *data)
{
	char	*result;
	char	*temp;
	int		i;

	if (!line || !data)
		return (ft_strdup(line));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (line[i])
	{
		temp = process_character(line, &i, data);
		result = append_to_result(result, temp);
	}
	return (result);
}

static void	process_heredoc_line(char *line, int quoted, t_data *data,
		int write_fd)
{
	char	*expanded;

	if (!quoted)
	{
		expanded = expand_heredoc_line(line, data);
		write_heredoc_line(write_fd, expanded);
		our_free(expanded);
	}
	else
		write_heredoc_line(write_fd, line);
}

int	handle_heredoc(const char *delimiter, int quoted, t_data *data)
{
	int					pipefd[2];
	char				*line;
	struct sigaction	old_action;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (setup_heredoc_signal_handler(&old_action) == -1)
	{
		close(pipefd[1]);
		close(pipefd[0]);
		return (-1);
	}
	while (1)
	{
		line = read_heredoc_line_input();
		if (!line)
		{
			if (g_signal_status == SIGINT)
			{
				close(pipefd[1]);
				close(pipefd[0]);
				data->heredoc_interrupted = 1;
				g_signal_status = 0;
				restore_signal_handler(&old_action);
				return (-1);
			}
			else
				break ;
		}
		if (is_delimiter(line, delimiter))
		{
			our_free(line);
			break ;
		}
		process_heredoc_line(line, quoted, data, pipefd[1]);
		our_free(line);
	}
	close(pipefd[1]);
	restore_signal_handler(&old_action);
	return (pipefd[0]);
}

int	setup_heredoc(t_cmd *cmd, t_data *data)
{
	if (!cmd->heredoc_limiter)
		return (0);
	if (cmd->heredoc_content)
		cmd->heredoc_fd = handle_heredoc_with_content(cmd->heredoc_content);
	else
		cmd->heredoc_fd = handle_heredoc(cmd->heredoc_limiter,
				cmd->heredoc_quoted, data);
	if (cmd->heredoc_fd == -1)
	{
		if (data->heredoc_interrupted)
			return (-2);
		return (-1);
	}
	if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 heredoc");
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
		return (-1);
	}
	close(cmd->heredoc_fd);
	cmd->heredoc_fd = -1;
	return (0);
}

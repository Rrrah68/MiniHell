/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 00:00:00 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc_interruption(int *pipefd, t_data *data,
		struct sigaction *old_action)
{
	if (g_signal_status == SIGINT)
	{
		close(pipefd[1]);
		close(pipefd[0]);
		data->heredoc_interrupted = 1;
		g_signal_status = 0;
		restore_signal_handler(old_action);
		return (-1);
	}
	return (0);
}

int	process_heredoc_input(t_heredoc_params *params)
{
	if (is_delimiter(params->line, params->delimiter))
	{
		our_free(params->line);
		return (1);
	}
	process_heredoc_line(params->line, params->quoted, params->data,
		params->write_fd);
	our_free(params->line);
	return (0);
}

void	process_heredoc_line(char *line, int quoted, t_data *data,
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

int	setup_heredoc_pipe_and_signals(int *pipefd,
		struct sigaction *old_action)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (setup_heredoc_signal_handler(old_action) == -1)
	{
		close(pipefd[1]);
		close(pipefd[0]);
		return (-1);
	}
	return (0);
}

int	read_heredoc_loop(t_heredoc_loop_params *p)
{
	char				*line;
	int					result;
	t_heredoc_params	params;

	while (1)
	{
		line = read_heredoc_line_input();
		if (!line)
		{
			result = handle_heredoc_interruption(p->pipefd, p->data,
					p->old_action);
			if (result == -1)
				return (-1);
			break ;
		}
		params = (t_heredoc_params){line, p->delimiter, p->quoted, p->data,
			p->pipefd[1]};
		result = process_heredoc_input(&params);
		if (result == 1)
			break ;
	}
	return (0);
}

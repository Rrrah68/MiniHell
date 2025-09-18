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

int	handle_heredoc(const char *delimiter, int quoted, t_data *data)
{
	int						pipefd[2];
	struct sigaction		old_action;
	int						result;
	t_heredoc_loop_params	loop_params;

	if (setup_heredoc_pipe_and_signals(pipefd, &old_action) == -1)
		return (-1);
	loop_params = (t_heredoc_loop_params){delimiter, quoted, data, pipefd,
		&old_action};
	result = read_heredoc_loop(&loop_params);
	if (result == -1)
		return (-1);
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

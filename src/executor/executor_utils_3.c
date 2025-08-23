/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:45:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/15 18:44:57 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_children(void)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		pid = wait(&status);
	}
}

int	process_cmd(t_cmd *cmd, int in_fd, t_data *data)
{
	int		p[2];
	int		out_fd;
	pid_t	pid;

	if (cmd->next)
	{
		if (setup_pipe_fd(cmd, p, &out_fd) == -1)
			return (-1);
	}
	else
		out_fd = STDOUT_FILENO;
	pid = fork_or_exit();
	if (pid == 0)
		exec_child(data, cmd, in_fd, out_fd);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (cmd->next)
	{
		close(p[1]);
		return (p[0]);
	}
	return (STDIN_FILENO);
}

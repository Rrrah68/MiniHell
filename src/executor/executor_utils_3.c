/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/15 00:00:00 by mobullad         ###   ########.fr       */
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

static int	setup_and_fork(t_fork_params *params, t_data *data)
{
	if (params->cmd->next)
	{
		if (setup_pipe_fd(params->cmd, params->p, params->out_fd) == -1)
			return (-1);
	}
	else
		*params->out_fd = STDOUT_FILENO;
	*params->pid = fork_or_exit();
	if (*params->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_DFL);
		if (params->cmd->next)
			close(params->p[0]);
		exec_child(data, params->cmd, params->in_fd, *params->out_fd);
	}
	return (0);
}

static int	parent_finalize(t_finalize_params *params, t_data *data)
{
	int	status;

	if (params->in_fd != STDIN_FILENO)
		close(params->in_fd);
	if (params->cmd->next)
	{
		close(params->p[1]);
		return (params->p[0]);
	}
	if (waitpid(params->pid, &status, 0) == -1)
		data->exit_status = 1;
	else if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_status = 128 + WTERMSIG(status);
	else
		data->exit_status = 1;
	return (STDIN_FILENO);
}

int	process_cmd(t_cmd *cmd, int in_fd, t_data *data)
{
	int					p[2];
	int					out_fd;
	pid_t				pid;
	t_fork_params		fork_params;
	t_finalize_params	final_params;

	fork_params.cmd = cmd;
	fork_params.in_fd = in_fd;
	fork_params.p = p;
	fork_params.out_fd = &out_fd;
	fork_params.pid = &pid;
	if (setup_and_fork(&fork_params, data) == -1)
		return (-1);
	final_params.cmd = cmd;
	final_params.in_fd = in_fd;
	final_params.p = p;
	final_params.pid = pid;
	return (parent_finalize(&final_params, data));
}

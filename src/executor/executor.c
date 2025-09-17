/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/17 14:38:07 by radahman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_redirection_and_check_args(t_cmd *cmd, t_data *data,
		int *in_backup, int *out_backup)
{
	backup_fds(in_backup, out_backup);
	if (setup_redirections(cmd, data) == -1)
	{
		restore_fds(*in_backup, *out_backup);
		if (data)
			data->exit_status = 1;
		return (-1);
	}
	if (!cmd->argv || !cmd->argv[0] || !cmd->argv[0][0])
		return (0);
	return (1);
}

static int	execute_builtin_with_fds(t_cmd *cmd, t_data *data,
		int in_backup, int out_backup)
{
	t_builtin	bi;
	int			status;

	bi = get_builtin(cmd->argv[0]);
	if (bi == BI_NONE)
		return (-1);
	if (bi == BI_CD || bi == BI_EXPORT || bi == BI_UNSET || bi == BI_EXIT)
		status = handle_builtin_parent(bi, cmd->argv, data);
	else
		status = handle_builtin_child(bi, cmd->argv, data);
	restore_fds(in_backup, out_backup);
	return (status);
}

static int	simplecmd_spawn_and_wait(t_cmd *cmd, t_data *data)
{
	pid_t	pid;

	pid = fork_or_exit();
	if (pid == 0)
		exec_child(data, cmd, STDIN_FILENO, STDOUT_FILENO);
	return (wait_child_with_signals(pid));
}

int	execute_simple_cmd(t_cmd *cmd, t_data *data)
{
	int	in_backup;
	int	out_backup;
	int	redirect_result;
	int	status;

	if (!cmd)
	{
		if (data)
			data->exit_status = 0;
		return (0);
	}
	redirect_result = handle_redirection_and_check_args(cmd, data,
			&in_backup, &out_backup);
	if (redirect_result == -1)
		return (1);
	if (redirect_result == 0)
	{
		restore_fds(in_backup, out_backup);
		if (data)
			data->exit_status = 0;
		return (0);
	}
	if (cmd->argv && cmd->argv[0] && is_variable_assignment(cmd->argv[0]))
	{
		status = handle_variable_assignment(cmd->argv[0], data);
		restore_fds(in_backup, out_backup);
		if (data)
			data->exit_status = status ? 0 : 1;
		return (status ? 0 : 1);
	}
	status = execute_builtin_with_fds(cmd, data, in_backup, out_backup);
	if (status != -1)
		return (status);
	status = simplecmd_spawn_and_wait(cmd, data);
	restore_fds(in_backup, out_backup);
	if (data)
		data->exit_status = status;
	return (status);
}

void	execute_all(t_cmd *cmds, t_data *data)
{
	int	in_fd;

	in_fd = STDIN_FILENO;
	while (cmds)
	{
		in_fd = process_cmd(cmds, in_fd, data);
		if (in_fd < 0)
		{
			if (cmds->next)
			{
				in_fd = STDIN_FILENO;
				cmds = cmds->next;
			}
			else
				return ;
		}
		else
			cmds = cmds->next;
	}
	wait_children(data);
}

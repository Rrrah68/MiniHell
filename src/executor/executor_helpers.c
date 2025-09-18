/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 18:06:47 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_builtin_with_fds(t_cmd *cmd, t_data *data, int in_backup,
		int out_backup)
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

int	handle_var_assignment_cmd(t_cmd *cmd, t_data *data, int in_backup,
		int out_backup)
{
	int	status;

	status = handle_variable_assignment(cmd->argv[0], data);
	restore_fds(in_backup, out_backup);
	if (data)
	{
		if (status)
			data->exit_status = 0;
		else
			data->exit_status = 1;
	}
	if (status)
		return (0);
	else
		return (1);
}

int	execute_cmd_final(t_cmd *cmd, t_data *data, int in_backup, int out_backup)
{
	int	status;

	status = execute_builtin_with_fds(cmd, data, in_backup, out_backup);
	if (status == -2)
	{
		restore_fds(in_backup, out_backup);
		return (-2);
	}
	if (status != -1)
		return (status);
	status = simplecmd_spawn_and_wait(cmd, data);
	restore_fds(in_backup, out_backup);
	if (data)
		data->exit_status = status;
	return (status);
}

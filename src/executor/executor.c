/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 18:05:11 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_redirection_and_check_args(t_cmd *cmd, t_data *data,
		int *in_backup, int *out_backup)
{
	int	result;

	backup_fds(in_backup, out_backup);
	result = setup_redirections(cmd, data);
	if (result == -1)
	{
		restore_fds(*in_backup, *out_backup);
		if (data)
			data->exit_status = 1;
		return (-1);
	}
	if (result == -2)
	{
		restore_fds(*in_backup, *out_backup);
		if (data)
			data->exit_status = 130;
		return (-2);
	}
	if (!cmd->argv || !cmd->argv[0] || !cmd->argv[0][0])
		return (0);
	return (1);
}

int	execute_simple_cmd(t_cmd *cmd, t_data *data)
{
	int	in_backup;
	int	out_backup;
	int	redirect_result;

	if (!cmd)
	{
		if (data)
			data->exit_status = 0;
		return (0);
	}
	redirect_result = handle_redirection_and_check_args(cmd, data, &in_backup,
			&out_backup);
	if (redirect_result == -1)
		return (1);
	if (redirect_result == -2)
		return (130);
	if (redirect_result == 0)
	{
		restore_fds(in_backup, out_backup);
		if (data)
			data->exit_status = 0;
		return (0);
	}
	if (cmd->argv && cmd->argv[0] && is_variable_assignment(cmd->argv[0]))
		return (handle_var_assignment_cmd(cmd, data, in_backup, out_backup));
	return (execute_cmd_final(cmd, data, in_backup, out_backup));
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

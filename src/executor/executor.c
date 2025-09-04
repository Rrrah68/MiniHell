/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:31:11 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/19 20:40:14 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	simplecmd_setup_and_builtins(t_cmd *cmd, t_data *data,
				int *in_backup, int *out_backup, int *status)
{
	t_builtin	bi;

	backup_fds(in_backup, out_backup);
	if (setup_redirections(cmd) == -1)
	{
		restore_fds(*in_backup, *out_backup);
		if (data)
			data->exit_status = 1;
		*status = 1;
		return (1);
	}
	bi = get_builtin(cmd->argv[0]);
	if (bi == BI_NONE)
		return (0);
	if (bi == BI_CD || bi == BI_EXPORT || bi == BI_UNSET || bi == BI_EXIT)
		*status = handle_builtin_parent(bi, cmd->argv, data);
	else
		*status = handle_builtin_child(bi, cmd->argv, data);
	restore_fds(*in_backup, *out_backup);
	return (1);
}

static int	simplecmd_spawn_and_wait(t_cmd *cmd, t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork_or_exit();
	if (pid == 0)
		exec_child(data, cmd, STDIN_FILENO, STDOUT_FILENO);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_simple_cmd(t_cmd *cmd, t_data *data)
{
	int	in_backup;
	int	out_backup;
	int	status;

	if (simplecmd_setup_and_builtins(cmd, data, &in_backup, &out_backup, &status))
		return (status);
	status = simplecmd_spawn_and_wait(cmd, data);
	restore_fds(in_backup, out_backup);
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
			return ;
		cmds = cmds->next;
	}
	wait_children();
}

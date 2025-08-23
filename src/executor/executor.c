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

int	execute_simple_cmd(t_cmd *cmd, t_data *data)
{
	pid_t		pid;
	int			status;
	int			in_backup;
	int			out_backup;
	t_builtin	bi;

	backup_fds(&in_backup, &out_backup);
	if (setup_redirections(cmd) == -1)
		return (-1);
	bi = get_builtin(cmd->argv[0]);
	if (bi != BI_NONE)
	{
		if (bi == BI_CD || bi == BI_EXPORT || bi == BI_UNSET || bi == BI_EXIT)
			return (handle_builtin_parent(bi, cmd->argv, data));
		return (handle_builtin_child(bi, cmd->argv, data));
	}
	pid = fork_or_exit();
	if (pid == 0)
		exec_child(data, cmd, STDIN_FILENO, STDOUT_FILENO);
	waitpid(pid, &status, 0);
	restore_fds(in_backup, out_backup);
	return (WEXITSTATUS(status));
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:55:48 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 17:29:00 by radahman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	fork_or_exit(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		safe_as_fuck_malloc(0,0,-1);
		exit(1);
	}
	return (pid);
}

void	backup_fds(int *in_backup, int *out_backup)
{
	*in_backup = dup(STDIN_FILENO);
	*out_backup = dup(STDOUT_FILENO);
	if (*in_backup < 0 || *out_backup < 0)
	{
		perror("dup");
		cleanup_data(NULL);
		exit(EXIT_FAILURE);
	}
}

void	restore_fds(int in_backup, int out_backup)
{
	redirect_and_close(in_backup, STDIN_FILENO);
	redirect_and_close(out_backup, STDOUT_FILENO);
}

int	handle_builtin_parent(t_builtin bi, char **argv, t_data *data)
{
	int	ret;
	int	in_backup;
	int	out_backup;

	backup_fds(&in_backup, &out_backup);
	ret = exec_builtin(bi, argv, data);
	restore_fds(in_backup, out_backup);
	return (ret);
}

int	handle_builtin_child(t_builtin bi, char **argv, t_data *data)
{
	pid_t	pid;
	int		status;
	int		in_backup;
	int		out_backup;

	backup_fds(&in_backup, &out_backup);
	pid = fork_or_exit();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_DFL);
		exec_builtin(bi, argv, data);
		cleanup_data(data);
		exit(0);
	}
	status = wait_child_with_signals(pid);
	restore_fds(in_backup, out_backup);
	return (status);
}

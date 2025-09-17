/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:55:44 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/16 15:55:45 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe_fd(t_cmd *cmd, int p[2], int *out_fd)
{
	if (cmd->next)
	{
		if (pipe(p) == -1)
		{
			perror("pipe");
			return (-1);
		}
		*out_fd = p[1];
	}
	else
	{
		*out_fd = STDOUT_FILENO;
	}
	return (0);
}

void	redirect_and_close(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		exit(1);
	}
	if (old_fd != new_fd)
	{
		close(old_fd);
	}
}

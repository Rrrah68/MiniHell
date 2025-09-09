/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rayandahmani <rayandahmani@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:31:06 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/09 18:37:04 by rayandahman      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_print_file_error(const char *path, int err)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(err), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (-1);
}

int	handle_infile(char *infile)
{
	int	fd;
	int	err;

	fd = open(infile, O_RDONLY);
	if (fd < 0)
		return ms_print_file_error(infile, errno);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		err = errno;
		close(fd);
		return ms_print_file_error(infile, err);
	}
	close(fd);
	return (0);
}

int	handle_outfile(char *outfile, int append)
{
	int	fd;

	fd = open_outfile(outfile, append);
	if (fd == -1)
	{
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 outfile");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	setup_redirections(t_cmd *cmd)
{
	if (cmd->in_precheck_failed && cmd->in_precheck_target)
		return (ms_print_file_error(cmd->in_precheck_target,
				cmd->in_precheck_errno));

	if (setup_heredoc(cmd) == -1)
		return (-1);
	if (cmd->infile != NULL && !cmd->heredoc_limiter)
	{
		if (handle_infile(cmd->infile) == -1)
			return (-1);
	}
	if (cmd->outfile != NULL)
	{
		if (handle_outfile(cmd->outfile, cmd->append) == -1)
			return (-1);
	}
	return (0);
}

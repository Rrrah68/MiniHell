/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:31:06 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/15 18:31:07 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_infile(char *infile)
{
	int	fd;

	fd = open_infile(infile);
	if (fd == -1)
	{
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 infile");
		close(fd);
		return (-1);
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

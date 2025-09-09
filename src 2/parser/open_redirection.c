/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:31:03 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/08 18:34:35 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_infile(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	return (fd);
}

int	get_outfile_flags(int append)
{
	if (append)
	{
		return (O_WRONLY | O_CREAT | O_APPEND);
	}
	return (O_WRONLY | O_CREAT | O_TRUNC);
}

int	open_outfile(char *outfile, int append)
{
	int	fd;
	int	flags;

	flags = get_outfile_flags(append);
	fd = open(outfile, flags, 0644);
	if (fd == -1)
	{
		perror(outfile);
		return (-1);
	}
	return (fd);
}

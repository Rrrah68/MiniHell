/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:56:24 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/16 15:56:25 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_precheck_infile(t_cmd *cur, const char *path)
{
	int	fd;

	if (cur->in_precheck_failed)
		return ;
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		cur->in_precheck_failed = 1;
		cur->in_precheck_errno = errno;
		cur->in_precheck_target = ft_strdup(path);
	}
	else
		close(fd);
}

void	ms_precheck_outfile(t_cmd *cur, const char *path, int do_append)
{
	int	fd;
	int	flags;

	if (cur->out_precheck_failed)
		return ;
	flags = O_WRONLY | O_CREAT;
	if (do_append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(path, flags, 0644);
	if (fd < 0)
	{
		cur->out_precheck_failed = 1;
		cur->out_precheck_errno = errno;
		cur->out_precheck_target = ft_strdup(path);
		return ;
	}
	close(fd);
}

int	ms_replace_str(char **dst, const char *src)
{
	char	*dup;

	dup = ft_strdup(src);
	if (!dup)
		return (0);
	if (*dst)
		free(*dst);
	*dst = dup;
	return (1);
}

int	cmd_set_infile(t_cmd *cur, const char *str)
{
	ms_precheck_infile(cur, str);
	if (!ms_replace_str(&cur->infile, str))
		return (0);
	if (cur->heredoc_limiter)
	{
		free(cur->heredoc_limiter);
		cur->heredoc_limiter = NULL;
	}
	return (1);
}

int	cmd_set_outfile(t_cmd *cur, const char *str, int append)
{
	ms_precheck_outfile(cur, str, append);
	if (!ms_replace_str(&cur->outfile, str))
		return (0);
	if (append)
		cur->append = 1;
	else
		cur->append = 0;
	return (1);
}

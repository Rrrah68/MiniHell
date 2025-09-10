/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:30:34 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/10 16:58:19 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_precheck_infile(t_cmd *cur, const char *path)
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

static void	ms_precheck_outfile(t_cmd *cur, const char *path, int do_append)
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

static int	ms_replace_str(char **dst, const char *src)
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

static int	cmd_set_infile(t_cmd *cur, const char *str)
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

static int	cmd_set_outfile(t_cmd *cur, const char *str, int append)
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

static int	cmd_set_heredoc_limiter(t_cmd *cur, const char *str)
{
	if (!ms_replace_str(&cur->heredoc_limiter, str))
		return (0);
	if (cur->infile)
	{
		free(cur->infile);
		cur->infile = NULL;
	}
	return (1);
}

static int	cmd_add_redir(t_cmd *cur, t_token_type type, char *str)
{
	if (!cur || !str)
		return (0);
	if (type == REDIRECT_IN)
		return (cmd_set_infile(cur, str));
	if (type == REDIRECT_OUT || type == REDIRECT_APPEND)
		return (cmd_set_outfile(cur, str, (type == REDIRECT_APPEND)));
	if (type == REDIRECT_HEREDOC)
		return (cmd_set_heredoc_limiter(cur, str));
	return (0);
}

void	*free_cmd_list(t_cmd *cmd)
{
	size_t	i;
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->argv)
		{
			i = 0;
			while (cmd->argv[i])
				free(cmd->argv[i++]);
			free(cmd->argv);
		}
		free(cmd->infile);
		free(cmd->outfile);
		free(cmd->heredoc_limiter);
		if (cmd->heredoc_fd > 0)
			close(cmd->heredoc_fd);
		free(cmd->in_precheck_target);
		free(cmd->out_precheck_target);
		free(cmd);
		cmd = next;
	}
	return (NULL);
}

t_cmd	*parser_error(t_cmd *cmds, const char *msg)
{
	(void)cmds;
	if (msg)
		ft_putstr_fd((char *)msg, 2);
	g_exit_status = 2;
	return (NULL);
}

int	handle_heredoc_parser(t_token **tok, t_cmd *cur)
{
	t_token	*next;

	next = (*tok)->next;
	if (!next || next->type != WORD)
		return (0);
	if (cur->heredoc_limiter)
		free(cur->heredoc_limiter);
	cur->heredoc_limiter = ft_strdup(next->str);
	if (!cur->heredoc_limiter)
		return (0);
	return (1);
}

int	handle_redir(t_token **tok, t_cmd *cur)
{
	t_token	*op;
	t_token	*file;

	op = *tok;
	if (!op)
		return (0);
	if (op->in_quotes == 1)
		return (0);
	if (op->type != REDIRECT_IN && op->type != REDIRECT_OUT
		&& op->type != REDIRECT_APPEND && op->type != REDIRECT_HEREDOC)
		return (0);
	file = op->next;
	while (file && file->type == WHITESPACE)
		file = file->next;
	if (!file || file->type != WORD || !file->str)
		return (0);
	if (!cmd_add_redir(cur, op->type, file->str))
		return (0);
	*tok = file;
	return (1);
}

int	handle_pipe(t_cmd **cmds, t_cmd **cur, t_token *tok)
{
	if (!tok || !tok->next || tok->next->type == PIPE)
		return (0);
	*cur = create_cmd();
	if (!*cur)
		return (-1);
	add_cmd(cmds, *cur);
	return (1);
}
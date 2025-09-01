/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:30:34 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/15 18:39:09 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell.h"


static void	ms_precheck_infile(t_cmd *cur, const char *path)
{
	int	fd;

	/* si déjà une erreur mémorisée, ne rien faire (on veut la 1ère) */
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

/* … ton cmd_add_redir existant, en ajoutant ceci pour les sorties … */
static int	cmd_add_redir(t_cmd *cur, t_token_type type, char *str)
{
	char	*dup;

	if (!cur || !str)
		return (0);
	if (type == REDIRECT_IN)
	{
		/* (tu as déjà ms_precheck_infile dans mes patches précédents) */
		ms_precheck_infile(cur, str);
		dup = ft_strdup(str);
		if (!dup)
			return (0);
		if (cur->infile)
			free(cur->infile);
		cur->infile = dup;
		if (cur->heredoc_limiter)
		{
			free(cur->heredoc_limiter);
			cur->heredoc_limiter = NULL;
		}
		return (1);
	}
	if (type == REDIRECT_OUT || type == REDIRECT_APPEND)
	{
		/* NEW: effets de bord immédiats, comme Bash */
		ms_precheck_outfile(cur, str, (type == REDIRECT_APPEND));

		dup = ft_strdup(str);
		if (!dup)
			return (0);
		if (cur->outfile)
			free(cur->outfile);
		cur->outfile = dup;
		if (type == REDIRECT_APPEND)
			cur->append = 1;
		else
			cur->append = 0;
		return (1);
	}
	if (type == HEREDOC)
	{
		dup = ft_strdup(str);
		if (!dup)
			return (0);
		if (cur->heredoc_limiter)
			free(cur->heredoc_limiter);
		cur->heredoc_limiter = dup;
		if (cur->infile)
		{
			free(cur->infile);
			cur->infile = NULL;
		}
		return (1);
	}
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
		/* NEW */
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

	/* ⛑ ne PAS traiter comme redirection si c’était dans des quotes
	   ou si ce n’est pas un opérateur standalone (str doit être NULL) */
	if (op->in_quotes == 1 || op->str != NULL)
		return (0);

	if (op->type != REDIRECT_IN && op->type != REDIRECT_OUT
		&& op->type != REDIRECT_APPEND && op->type != HEREDOC)
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
	/* tok pointe sur le token PIPE. On veut s'assurer qu'il y a quelque chose après */
	if (!tok || !tok->next || tok->next->type == PIPE)
		return (0);

	*cur = create_cmd();
	if (!*cur)
		return (-1);

	add_cmd(cmds, *cur);
	return (1);
}
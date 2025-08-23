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
	if (op->type != REDIRECT_IN && op->type != REDIRECT_OUT
		&& op->type != REDIRECT_APPEND && op->type != HEREDOC)
		return (0);
	file = op->next;
	while (file && file->type == WHITESPACE)
		file = file->next;
	if (!file || file->type != WORD || !file->str)
		return (0);

	/* Enregistre la redirection (remplace par ta fonction) */
	if (!cmd_add_redir(cur, op->type, file->str))
		return (0);

	/* IMPORTANT : Consommer jusqu’au filename.
	   La boucle extérieure fera ensuite tok = tok->next. */
	*tok = file;
	return (1);
}

int	handle_pipe(t_cmd **cmds, t_cmd **cur, t_token *tok)
{
	if (!tok->next || tok->next->type == PIPE)
		return (0);
	*cur = create_cmd();
	if (!*cur)
		return (-1);
	add_cmd(cmds, *cur);
	return (1);
}

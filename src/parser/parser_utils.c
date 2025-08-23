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
	if (msg)
		ft_putstr_fd((char *)msg, 2);
	free_cmd_list(cmds);
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
	int		append;
	t_token	*next;

	append = ((*tok)->type == REDIRECT_APPEND);
	next = (*tok)->next;
	if (!next || next->type != WORD)
		return (0);
	*tok = next;
	if ((*tok)->type == REDIRECT_HEREDOC)
	{
		if (!handle_heredoc_parser(tok, cur))
			return (0);
	}
	if (append || (*tok)->type == REDIRECT_OUT)
	{
		free(cur->outfile);
		cur->outfile = ft_strdup(next->str);
		cur->append = append;
	}
	else
	{
		free(cur->infile);
		cur->infile = ft_strdup(next->str);
	}
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

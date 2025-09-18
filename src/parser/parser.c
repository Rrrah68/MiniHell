/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:56:21 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 21:13:51 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_redirect(t_token **tok, t_cmd *cur, t_cmd *cmds)
{
	if (!handle_redir(tok, cur))
	{
		parser_error(cmds, "minishell: syntax error near redirect\n");
		return (0);
	}
	return (1);
}

int	process_pipe(t_token *tok, t_cmd **cmds, t_cmd **cur)
{
	int	status;

	status = handle_pipe(cmds, cur, tok);
	if (status <= 0)
	{
		if (status == 0)
		{
			return (0);
		}
		else
			parser_error(*cmds, "minishell: malloc error\n");
		return (0);
	}
	return (1);
}

int	process_token(t_token **tok, t_cmd **cmds, t_cmd **cur)
{
	if ((*tok)->type == WORD)
		(*cur)->argv = append_to_argv((*cur)->argv, (*tok)->str);
	else if ((*tok)->type == REDIRECT_IN || (*tok)->type == REDIRECT_OUT
		|| (*tok)->type == REDIRECT_APPEND || (*tok)->type == REDIRECT_HEREDOC)
	{
		if (!process_redirect(tok, *cur, *cmds))
			return (0);
	}
	else if ((*tok)->type == PIPE)
	{
		if (!process_pipe(*tok, cmds, cur))
			return (0);
	}
	return (1);
}

t_cmd	*parse_token(t_token *lexer)
{
	t_cmd	*cmds;
	t_cmd	*cur;
	t_token	*tok;

	cmds = create_cmd();
	if (!cmds)
		return (NULL);
	cur = cmds;
	tok = lexer;
	while (tok)
	{
		if (!process_token(&tok, &cmds, &cur))
		{
			free_cmd_list(cmds);
			return (NULL);
		}
		tok = tok->next;
	}
	return (cmds);
}

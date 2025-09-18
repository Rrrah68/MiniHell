/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:56:23 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 18:03:28 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_immediate_heredoc(const char *delimiter, int quoted)
{
	char	*line;

	(void)quoted;
	while (1)
	{
		line = read_heredoc_line_input();
		if (!line)
			break ;
		if (is_delimiter(line, delimiter))
		{
			our_free(line);
			break ;
		}
		our_free(line);
	}
}

int	cmd_set_heredoc_limiter(t_cmd *cur, const char *str, int quoted)
{
	if (cur->heredoc_limiter)
	{
		process_immediate_heredoc(cur->heredoc_limiter, cur->heredoc_quoted);
		our_free(cur->heredoc_limiter);
		cur->heredoc_limiter = NULL;
	}
	if (!ms_replace_str(&cur->heredoc_limiter, str))
		return (0);
	cur->heredoc_quoted = quoted;
	if (cur->infile)
	{
		our_free(cur->infile);
		cur->infile = NULL;
	}
	return (1);
}

int	cmd_add_redir(t_cmd *cur, t_token_type type, char *str, int quoted)
{
	if (!cur || !str)
		return (0);
	if (type == REDIRECT_IN)
		return (cmd_set_infile(cur, str));
	if (type == REDIRECT_OUT || type == REDIRECT_APPEND)
		return (cmd_set_outfile(cur, str, (type == REDIRECT_APPEND)));
	if (type == REDIRECT_HEREDOC)
		return (cmd_set_heredoc_limiter(cur, str, quoted));
	return (0);
}

void	free_cmd_list(t_cmd *cmd)
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
				our_free(cmd->argv[i++]);
			our_free(cmd->argv);
		}
		our_free(cmd->infile);
		our_free(cmd->outfile);
		our_free(cmd->heredoc_limiter);
		our_free(cmd->heredoc_content);
		if (cmd->heredoc_fd >= 0)
			close(cmd->heredoc_fd);
		our_free(cmd->in_precheck_target);
		our_free(cmd->out_precheck_target);
		our_free(cmd);
		cmd = next;
	}
}

void	cleanup_data(t_data *data)
{
	safe_as_fuck_malloc(0, 0, SAFE_MALLOC_FREE_ALL);
	if (data != NULL)
	{
		data->input = NULL;
		data->prompt = NULL;
		data->lexer = NULL;
		data->cmds = NULL;
		data->env = NULL;
	}
}

int	handle_heredoc_parser(t_token **tok, t_cmd *cur)
{
	t_token	*next;

	next = (*tok)->next;
	if (!next || next->type != WORD)
		return (0);
	if (cur->heredoc_limiter)
		our_free(cur->heredoc_limiter);
	cur->heredoc_limiter = ft_strdup(next->str);
	if (!cur->heredoc_limiter)
		return (0);
	cur->heredoc_quoted = (next->in_quotes == 1);
	return (1);
}

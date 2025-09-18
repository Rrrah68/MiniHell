/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:56:23 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 16:23:31 by radahman         ###   ########.fr       */
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
			free(line);
			break ;
		}
		free(line);
	}
}

int	cmd_set_heredoc_limiter(t_cmd *cur, const char *str, int quoted)
{
	if (cur->heredoc_limiter)
	{
		process_immediate_heredoc(cur->heredoc_limiter, cur->heredoc_quoted);
		free(cur->heredoc_limiter);
		cur->heredoc_limiter = NULL;
	}
	if (!ms_replace_str(&cur->heredoc_limiter, str))
		return (0);
	cur->heredoc_quoted = quoted;
	if (cur->infile)
	{
		free(cur->infile);
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
				free(cmd->argv[i++]);
			free(cmd->argv);
		}
		free(cmd->infile);
		free(cmd->outfile);
		free(cmd->heredoc_limiter);
		free(cmd->heredoc_content);
		if (cmd->heredoc_fd >= 0)
			close(cmd->heredoc_fd);
		free(cmd->in_precheck_target);
		free(cmd->out_precheck_target);
		free(cmd);
		cmd = next;
	}
}

void	cleanup_data(t_data *data)
{
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
	if (data->prompt)
	{
		free(data->prompt);
		data->prompt = NULL;
	}
	if (data->lexer)
	{
		free_tokens(data->lexer);
		data->lexer = NULL;
	}
	if (data->cmds)
	{
		free_cmd_list(data->cmds);
		data->cmds = NULL;
	}
	if (data->env)
	{
		free_environment(data->env);
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
		free(cur->heredoc_limiter);
	cur->heredoc_limiter = ft_strdup(next->str);
	if (!cur->heredoc_limiter)
		return (0);
	cur->heredoc_quoted = (next->in_quotes == 1);
	return (1);
}

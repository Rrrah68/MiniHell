








/* ************************************************************************** */

#include "minishell.h"

int	cmd_set_heredoc_limiter(t_cmd *cur, const char *str)
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

int	cmd_add_redir(t_cmd *cur, t_token_type type, char *str)
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
		free(cmd->heredoc_content);
		if (cmd->heredoc_fd >= 0)
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
	cur->heredoc_quoted = (next->in_quotes == 1);
	return (1);
}

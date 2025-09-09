/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redirections.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rayandahmani <rayandahmani@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:30:34 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/09 18:34:25 by rayandahman      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

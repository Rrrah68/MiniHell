/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:30:37 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/15 18:32:57 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return (cmd);
}

void	add_cmd(t_cmd **list, t_cmd *new_cmd)
{
	t_cmd	*cur;

	if (!list || !new_cmd)
		return ;
	if (!*list)
	{
		*list = new_cmd;
		return ;
	}
	cur = *list;
	while (cur->next)
		cur = cur->next;
	cur->next = new_cmd;
}

char	**append_to_argv(char **argv, char *word)
{
	size_t	len;
	size_t	i;
	char	**new_argv;

	i = 0;
	len = 0;
	if (!word)
		return (argv);
	while (argv && argv[len])
		len++;
	new_argv = ft_calloc(len + 2, sizeof(char *));
	if (!new_argv)
		return (argv);
	while (i < len)
	{
		new_argv[i] = ft_strdup(argv[i]);
		free(argv[i]);
		i++;
	}
	new_argv[i++] = ft_strdup(word);
	new_argv[i] = NULL;
	free(argv);
	return (new_argv);
}

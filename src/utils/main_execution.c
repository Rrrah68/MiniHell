/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:35:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 21:07:41 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*parse_input(t_data *data)
{
	t_cmd	*cmds;

	data->cmds = NULL;
	lexer(data, data->input);
	if (data->lexer)
	{
		cmds = parse_token(data->lexer);
		data->cmds = cmds;
		free_tokens(data->lexer);
		data->lexer = NULL;
	}
	else
		cmds = NULL;
	return (cmds);
}

static int	execute_commands(t_cmd *cmds, t_data *data)
{
	int	exec_result;

	if (cmds->next)
		execute_all(cmds, data);
	else
	{
		exec_result = execute_simple_cmd(cmds, data);
		if (exec_result == -2)
		{
			free_cmd_list(cmds);
			data->cmds = NULL;
			return (-2);
		}
	}
	return (0);
}

int	process_and_execute(t_data *data)
{
	t_cmd	*cmds;
	int		result;

	cmds = parse_input(data);
	if (data->input)
	{
		our_free(data->input);
		data->input = NULL;
	}
	if (cmds)
	{
		result = execute_commands(cmds, data);
		if (result == -2)
			return (-2);
		free_cmd_list(cmds);
		data->cmds = NULL;
	}
	return (0);
}

int	main_loop(t_data *data)
{
	int	result;

	while (1)
	{
		signal(SIGINT, signal_handler);
		get_prompt(data);
		result = handle_input(data);
		if (result == 0)
			break ;
		if (result == 2)
		{
			write(STDOUT_FILENO, "\n", 1);
			continue ;
		}
		result = process_and_execute(data);
		if (result == -2)
			break ;
	}
	return (data->exit_status);
}

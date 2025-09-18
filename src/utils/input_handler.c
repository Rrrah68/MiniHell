/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:30:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 21:13:47 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_signal_status;

static void	append_continuation_line(char **complete_input, char *line)
{
	char	*temp;

	temp = ft_strjoin(*complete_input, " ");
	our_free(*complete_input);
	*complete_input = ft_strjoin(temp, line);
	our_free(temp);
	our_free(line);
}

int	handle_continuation_input(char **complete_input)
{
	char	*line;
	void	(*old_handler)(int);

	old_handler = signal(SIGINT, continuation_signal_handler);
	g_signal_status = 0;
	line = readline("> ");
	signal(SIGINT, old_handler);
	if (g_signal_status == SIGINT || !line)
	{
		if (line)
			our_free(line);
		our_free(*complete_input);
		*complete_input = NULL;
		g_signal_status = 0;
		return (0);
	}
	append_continuation_line(complete_input, line);
	return (1);
}

static int	process_incomplete_input(char **complete_input)
{
	int		len;
	int		i;

	if (!*complete_input)
		return (1);
		
	len = ft_strlen(*complete_input);
	if (len == 0)
		return (1);
		
	i = len - 1;
	while (i >= 0 && (*complete_input)[i] == ' ')
		i--;
		
	if (i >= 0 && (*complete_input)[i] == '|')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
		return (1);
	}
	
	return (1);
}

static int	check_exit_command(char *input)
{
	if (ft_strncmp(input, "exit", 5) == 0)
	{
		our_free(input);
		return (0);
	}
	return (1);
}

int	handle_input(t_data *data)
{
	char	*complete_input;
	int		result;

	data->input = readline(data->prompt);
	update_exit_status(data);
	if (!data->input)
	{
		write(1, "exit\n", 5);
		return (0);
	}
	complete_input = ft_strdup(data->input);
	our_free(data->input);
	result = process_incomplete_input(&complete_input);
	if (result != 1)
	{
		if (complete_input)
			our_free(complete_input);
		return (result);
	}
	data->input = complete_input;
	if (data->input[0])
		add_history(data->input);
	return (check_exit_command(data->input));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:45:34 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/10 15:40:44 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_prompt(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	if (data->prompt)
		free(data->prompt);
	data->prompt = ft_strjoin(cwd, "$ ");
	free(cwd);
}

char	*get_type(t_token_type type)
{
	if (type == CHAR)
		return ("CHAR");
	else if (type == SYMBOL)
		return ("SYMBOL");
	else if (type == WHITESPACE)
		return ("WHITESPACE");
	else if (type == SINGLE_QUOTE)
		return ("SIMPLE_QUOTE");
	else if (type == DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	else if (type == WORD)
		return ("WORD");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == REDIRECT_IN)
		return ("REDIRECT_IN");
	else if (type == REDIRECT_OUT)
		return ("REDIRECT_OUT");
	else if (type == REDIRECT_APPEND)
		return ("REDIRECT_APPEND");
	else if (type == REDIRECT_HEREDOC)
		return ("REDIRECT_HEREDOC");
	return ("UNKNOWN");
}

volatile sig_atomic_t	g_signal_status = 0;
int						g_exit_status = 0;

static int	handle_input(t_data *data)
{
	data->input = readline(data->prompt);
	if (!data->input)
	{
		write(1, "exit\n", 5);
		return (0);
	}
	update_exit_status(data);
	if (data->input[0])
		add_history(data->input);
	if (ft_strncmp(data->input, "exit", 5) == 0)
	{
		free(data->input);
		return (0);
	}
	return (1);
}

static void	process_and_execute(t_data *data)
{
	t_cmd	*cmds;

	lexer(data, data->input);
	if (data->lexer)
	{
		cmds = parse_token(data->lexer);
		if (cmds)
		{
			if (cmds->next)
				execute_all(cmds, data);
			else
				execute_simple_cmd(cmds, data);
			free_cmd_list(cmds);
		}
		free_tokens(data->lexer);
	}
	data->lexer = NULL;
	free(data->input);
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	signal(SIGQUIT, SIG_IGN);
	(void)ac;
	(void)av;
	init_data(&data, envp);
	if (!data.env)
	{
		ft_putstr_fd("Error: environment init failed\n", STDERR_FILENO);
		exit(1);
	}
	while (1)
	{
		signal(SIGINT, signal_handler);
		get_prompt(&data);
		if (!handle_input(&data))
			break ;
		process_and_execute(&data);
	}
	free_environment(data.env);
	free(data.prompt);
	return (data.exit_status);
}

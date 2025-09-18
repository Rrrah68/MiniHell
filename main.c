/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:45:34 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/17 18:26:19 by mobullad         ###   ########.fr       */
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

static int	handle_continuation_input(char **complete_input)
{
	char	*line;
	char	*temp;
	void	(*old_handler)(int);
	
	// Installer le gestionnaire spécial pour la continuation
	old_handler = signal(SIGINT, continuation_signal_handler);
	g_signal_status = 0;
	
	line = readline("> ");
	
	// Restaurer le gestionnaire normal
	signal(SIGINT, old_handler);
	
	// Si le signal a été reçu pendant readline
	if (g_signal_status == SIGINT)
	{
		if (line)
			free(line);
		free(*complete_input);
		*complete_input = ft_strdup("");
		g_signal_status = 0;
		// Ne rien afficher ici, laisser la boucle principale s'en charger
		return (0);  // Sortir de la boucle de continuation
	}
	
	// Si readline retourne NULL (EOF avec Ctrl+D)
	if (!line)
	{
		free(*complete_input);
		write(1, "exit\n", 5);
		return (-1);  // Sortir du programme
	}
	
	// Ajouter la ligne à l'input complet
	temp = ft_strjoin(*complete_input, " ");
	free(*complete_input);
	*complete_input = ft_strjoin(temp, line);
	free(temp);
	free(line);
	
	return (1);  // Continuer
}

static int	handle_input(t_data *data)
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
	free(data->input);
	
	// Continuer à lire si l'input est incomplet (se termine par un pipe)
	while (is_incomplete_input(complete_input))
	{
		result = handle_continuation_input(&complete_input);
		if (result == -1)
			return (0);  // EOF - sortir du programme
		if (result == 0)
		{
			// Ctrl+C - nettoyer et retourner 2 pour indiquer une interruption
			free(complete_input);
			return (2);  // Signal d'interruption
		}
	}
	
	data->input = complete_input;
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

	data->cmds = NULL;
	lexer(data, data->input);
	if (data->lexer)
	{
		cmds = parse_token(data->lexer);
		data->cmds = cmds;  // Stocker dans data pour le nettoyage
		free_tokens(data->lexer);
		data->lexer = NULL;
	}
	else
		cmds = NULL;
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
	if (cmds)
	{
		if (cmds->next)
			execute_all(cmds, data);
		else
			execute_simple_cmd(cmds, data);
		free_cmd_list(cmds);
		data->cmds = NULL;  // Réinitialiser après libération
	}
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;
	int		result;

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
		result = handle_input(&data);
		if (result == 0)
			break ;  // EOF - sortir du programme
		if (result == 2)
		{
			// Interruption - afficher immédiatement le nouveau prompt avec write
			write(STDOUT_FILENO, data.prompt, ft_strlen(data.prompt));
			continue ;  // Continuer la boucle
		}
		process_and_execute(&data);
	}
	cleanup_data(&data);  // Nettoyer toutes les données restantes
	free_environment(data.env);
	free(data.prompt);
	return (data.exit_status);
}

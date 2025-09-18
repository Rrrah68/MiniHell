/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:45:34 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 22:23:13 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_prompt(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		cleanup_data(data);
		perror("getcwd");
		exit(1);
	}
	if (data->prompt)
		our_free(data->prompt);
	data->prompt = ft_strjoin(cwd, "$ ");
	our_free(cwd);
}

volatile sig_atomic_t	g_signal_status = 0;

int	main(int ac, char **av, char **envp)
{
	t_data	data;
	int		exit_status;

	signal(SIGQUIT, SIG_IGN);
	(void)ac;
	(void)av;
	init_data(&data, envp);
	if (!data.env)
	{
		ft_putstr_fd("Error: environment init failed\n", STDERR_FILENO);
		cleanup_data(&data);
		exit(1);
	}
	exit_status = main_loop(&data);
	cleanup_data(&data);
	return (exit_status);
}

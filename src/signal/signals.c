/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:56:29 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 21:13:12 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_status = sig;
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	continuation_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_status = sig;
		write(STDOUT_FILENO, "\n", 1);
		rl_done = 1;
	}
}

void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_status = sig;
		write(STDOUT_FILENO, "\n", 1);
	}
}

int	setup_heredoc_signal_handler(struct sigaction *old_action)
{
	struct sigaction	new_action;

	sigemptyset(&new_action.sa_mask);
	new_action.sa_handler = heredoc_signal_handler;
	new_action.sa_flags = 0;
	if (sigaction(SIGINT, &new_action, old_action) == -1)
		return (-1);
	return (0);
}

int	restore_signal_handler(struct sigaction *old_action)
{
	if (sigaction(SIGINT, old_action, NULL) == -1)
		return (-1);
	return (0);
}

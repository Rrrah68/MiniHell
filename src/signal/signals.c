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

int	wait_child_with_signals(pid_t pid)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit\n", 5);
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

void	update_exit_status(t_data *data)
{
	if (g_signal_status != 0)
	{
		data->exit_status = 128 + g_signal_status;
		g_signal_status = 0;
	}
}

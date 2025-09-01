/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:45:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/15 18:44:57 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_children(void)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		pid = wait(&status);
	}
}

int	process_cmd(t_cmd *cmd, int in_fd, t_data *data)
{
	int		p[2];
	int		out_fd;
	pid_t	pid;

	if (cmd->next)
	{
		if (setup_pipe_fd(cmd, p, &out_fd) == -1)
			return (-1);
	}
	else
		out_fd = STDOUT_FILENO;

	pid = fork_or_exit();
	if (pid == 0)
	{
		/* enfant : s'assurer que SIGPIPE est par défaut
		   (sinon pas de "Broken pipe" dans d'autres cas) */
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_DFL);

		/* si on écrit vers un pipe, on ne garde pas l'extrémité lecture */
		if (cmd->next)
			close(p[0]);

		exec_child(data, cmd, in_fd, out_fd);
	}

	/* parent */
	if (in_fd != STDIN_FILENO)
		close(in_fd);

	if (cmd->next)
	{
		/* parent : ne garde pas l'extrémité écriture */
		close(p[1]);
		/* passe la lecture au prochain maillon */
		return (p[0]);
	}
	else
	{
		/* DERNIER maillon : attendre ce pid et récupérer SON status */
		int status;

		if (waitpid(pid, &status, 0) == -1)
			data->exit_status = 1;
		else if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->exit_status = 128 + WTERMSIG(status);
		else
			data->exit_status = 1;

		/* la boucle appelera ensuite wait_children() pour les autres */
		return (STDIN_FILENO);
	}
}
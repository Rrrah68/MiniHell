/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_wait_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 21:38:41 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_status_from_signal(int status)
{
	if (WTERMSIG(status) == SIGINT)
		return (128 + SIGINT);
	else if (WTERMSIG(status) == SIGQUIT)
		return (128 + SIGQUIT);
	else if (WTERMSIG(status) == SIGPIPE)
		return (0);
	else
		return (128 + WTERMSIG(status));
}

int	process_child_status(int status)
{
	if (WIFSIGNALED(status))
		return (get_exit_status_from_signal(status));
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

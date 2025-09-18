/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:54:37 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 22:23:21 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_builtin	get_builtin(const char *cmd)
{
	if (!cmd)
		return (BI_NONE);
	if (ft_strncmp(cmd, "cd", 2) == 0 && (cmd[2] == '\0' || cmd[2] == ' '))
		return (BI_CD);
	if (ft_strncmp(cmd, "echo", 4) == 0 && (cmd[4] == '\0' || cmd[4] == ' '))
		return (BI_ECHO);
	if (ft_strncmp(cmd, "pwd", 3) == 0 && (cmd[3] == '\0' || cmd[3] == ' '))
		return (BI_PWD);
	if (ft_strncmp(cmd, "export", 6) == 0 && (cmd[6] == '\0' || cmd[6] == ' '))
		return (BI_EXPORT);
	if (ft_strncmp(cmd, "unset", 5) == 0 && (cmd[5] == '\0' || cmd[5] == ' '))
		return (BI_UNSET);
	if (ft_strncmp(cmd, "env", 3) == 0 && (cmd[3] == '\0' || cmd[3] == ' '))
		return (BI_ENV);
	if (ft_strncmp(cmd, "exit", 4) == 0 && (cmd[4] == '\0' || cmd[4] == ' '))
		return (BI_EXIT);
	return (BI_NONE);
}

int	exec_builtin(t_builtin bi, char **argv, t_data *data)
{
	if (bi == BI_ECHO)
		return (builtin_echo(argv));
	if (bi == BI_CD)
		return (builtin_cd(argv, data));
	if (bi == BI_PWD)
		return (builtin_pwd(argv, data));
	if (bi == BI_EXPORT)
		return (builtin_export(argv, data));
	if (bi == BI_UNSET)
		return (builtin_unset(argv, data));
	if (bi == BI_ENV)
		return (builtin_env(argv, data));
	if (bi == BI_EXIT)
		return (builtin_exit(argv, data));
	return (1);
}

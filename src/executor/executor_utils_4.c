/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 17:07:37 by radahman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_exit_code(int error_type, char *program_path)
{
	struct stat	st;

	if (error_type == ENOENT || error_type == -2)
		return (127);
	else if (error_type == -1)
		return (126);
	else if (error_type == EACCES)
	{
		if (program_path && stat(program_path, &st) == 0
			&& S_ISDIR(st.st_mode))
			return (126);
		else
			return (126);
	}
	return (127);
}

static void	print_error_message(char *cmd_name, int error_type,
		char *program_path)
{
	struct stat	st;

	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (error_type == ENOENT || error_type == -2)
		ft_putstr_fd("command not found\n", STDERR_FILENO);
	else if (error_type == -1)
		ft_putstr_fd("Is a directory\n", STDERR_FILENO);
	else if (error_type == EACCES)
	{
		if (program_path && stat(program_path, &st) == 0
			&& S_ISDIR(st.st_mode))
			ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		else
			ft_putstr_fd("Permission denied\n", STDERR_FILENO);
	}
	else
		ft_putstr_fd("execve: error\n", STDERR_FILENO);
}

static void	cleanup_and_exit(t_cleanup_params *params, int exit_code)
{
	cleanup_data(params->data);
	exit(exit_code);
}

void	print_exec_error_and_exit(t_exec_error_params *params)
{
	t_cleanup_params	cleanup_params;
	int					exit_code;

	print_error_message(params->cmd_name, params->error_type,
		params->program_path);
	exit_code = get_exit_code(params->error_type, params->program_path);
	cleanup_params.program_path = params->program_path;
	cleanup_params.env_array = params->env_array;
	cleanup_params.data = params->data;
	cleanup_params.cmd = params->cmd;
	cleanup_and_exit(&cleanup_params, exit_code);
}

char	*prepare_program_path(t_cmd *cmd, t_env *env, t_data *data,
		int *has_slash)
{
	char				*program_path;
	struct stat			st;
	t_exec_error_params	params;

	*has_slash = (ft_strchr(cmd->argv[0], '/') != NULL);
	program_path = find_program_path(cmd->argv[0], env);
	if (!program_path && !*has_slash)
	{
		params = (t_exec_error_params){cmd->argv[0], NULL, NULL, data, cmd, -2};
		print_exec_error_and_exit(&params);
	}
	if (!program_path)
		program_path = ft_strdup(cmd->argv[0]);
	if (*has_slash && stat(program_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		params = (t_exec_error_params){cmd->argv[0], program_path, NULL,
			data, cmd, -1};
		print_exec_error_and_exit(&params);
	}
	return (program_path);
}

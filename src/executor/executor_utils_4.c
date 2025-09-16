/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/16 19:41:46 by mobullad         ###   ########.fr       */
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

static void	cleanup_and_exit(char *program_path, char **env_array,
		int exit_code)
{
	if (program_path)
		free(program_path);
	if (env_array)
		ft_free_tab(env_array);
	exit(exit_code);
}

void	print_exec_error_and_exit(char *cmd_name, char *program_path,
		char **env_array, int error_type)
{
	int	exit_code;

	print_error_message(cmd_name, error_type, program_path);
	exit_code = get_exit_code(error_type, program_path);
	cleanup_and_exit(program_path, env_array, exit_code);
}

char	*prepare_program_path(t_cmd *cmd, t_env *env, int *has_slash)
{
	char		*program_path;
	struct stat	st;

	*has_slash = (ft_strchr(cmd->argv[0], '/') != NULL);
	program_path = find_program_path(cmd->argv[0], env);
	if (!program_path && !*has_slash)
		print_exec_error_and_exit(cmd->argv[0], NULL, NULL, -2);
	if (!program_path)
		program_path = ft_strdup(cmd->argv[0]);
	if (*has_slash && stat(program_path, &st) == 0 && S_ISDIR(st.st_mode))
		print_exec_error_and_exit(cmd->argv[0], program_path, NULL, -1);
	return (program_path);
}

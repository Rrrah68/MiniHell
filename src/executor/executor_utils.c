/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:31:13 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/10 15:28:25 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_path_directories(char **paths, char *program)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			break ;
		full_path = ft_strjoin(temp, program);
		free(temp);
		if (full_path && access(full_path, X_OK) == 0)
		{
			ft_free_tab(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_program_path(char *program, t_env *env)
{
	char	*path_var;
	char	**paths;
	char	*result;

	if (!program || !env)
		return (NULL);
	if (ft_strchr(program, '/'))
		return (ft_strdup(program));
	path_var = get_env_value(env, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	result = check_path_directories(paths, program);
	if (!result)
		ft_free_tab(paths);
	return (result);
}

static void	child_setup_io_and_redirs(t_cmd *cmd, int in_fd, int out_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	redirect_and_close(in_fd, STDIN_FILENO);
	redirect_and_close(out_fd, STDOUT_FILENO);
	if (setup_redirections(cmd) == -1)
		_exit(1);
}

static void	cleanup_and_exit(char *program_path, char **env_array, int exit_code)
{
	if (program_path)
		free(program_path);
	if (env_array)
		ft_free_tab(env_array);
	_exit(exit_code);
}

static void	print_exec_error_and_exit(char *cmd_name, char *program_path, char **env_array, int error_type)
{
	struct stat	st;

	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	
	if (error_type == ENOENT)
	{
		ft_putstr_fd("command not found\n", STDERR_FILENO);
		cleanup_and_exit(program_path, env_array, 127);
	}
	else if (error_type == EACCES)
	{
		/* Check if it's a directory for EACCES error */
		if (program_path && stat(program_path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			ft_putstr_fd("Is a directory\n", STDERR_FILENO);
			cleanup_and_exit(program_path, env_array, 126);
		}
		else
		{
			ft_putstr_fd("Permission denied\n", STDERR_FILENO);
			cleanup_and_exit(program_path, env_array, 126);
		}
	}
	else if (error_type == -1) /* Special case for directory check */
	{
		ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		cleanup_and_exit(program_path, env_array, 126);
	}
	else if (error_type == -2) /* Special case for command not found */
	{
		ft_putstr_fd("command not found\n", STDERR_FILENO);
		cleanup_and_exit(program_path, env_array, 127);
	}
	else
	{
		ft_putstr_fd("execve: error\n", STDERR_FILENO);
		cleanup_and_exit(program_path, env_array, 127);
	}
}

static void	child_run_exec(t_data *data, t_cmd *cmd)
{
	char		*program_path;
	char		**env_array;
	t_env		*env;
	t_builtin	bi;
	int			ret;
	struct stat	st;
	int			has_slash;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!cmd || !cmd->argv || !cmd->argv[0])
		_exit(127);
	bi = get_builtin(cmd->argv[0]);
	if (bi != BI_NONE)
	{
		ret = exec_builtin(bi, cmd->argv, data);
		_exit(ret);
	}
	env = NULL;
	if (data)
		env = data->env;
	has_slash = (ft_strchr(cmd->argv[0], '/') != NULL);
	program_path = find_program_path(cmd->argv[0], env);
	
	/* If no path found and command doesn't contain '/', it's not in PATH */
	if (!program_path && !has_slash)
		print_exec_error_and_exit(cmd->argv[0], NULL, NULL, -2);
	
	if (!program_path)
		program_path = ft_strdup(cmd->argv[0]);
	
	/* Only check if it's a directory for commands with '/' */
	if (has_slash && stat(program_path, &st) == 0 && S_ISDIR(st.st_mode))
		print_exec_error_and_exit(cmd->argv[0], program_path, NULL, -1);
	
	env_array = env_to_array(env);
	execve(program_path, cmd->argv, env_array);
	
	/* Handle execve errors */
	print_exec_error_and_exit(cmd->argv[0], program_path, env_array, errno);
}

void	exec_child(t_data *data, t_cmd *cmd, int in_fd, int out_fd)
{
	child_setup_io_and_redirs(cmd, in_fd, out_fd);
	child_run_exec(data, cmd);
}

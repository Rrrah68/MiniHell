/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:55:47 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 17:02:23 by radahman         ###   ########.fr       */
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
		our_free(temp);
		if (full_path && access(full_path, X_OK) == 0)
		{
			ft_free_tab(paths);
			return (full_path);
		}
		our_free(full_path);
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

static void	child_setup_io_and_redirs(t_cmd *cmd, int in_fd, int out_fd,
				t_data *data)
{
	int	result;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	redirect_and_close(in_fd, STDIN_FILENO);
	redirect_and_close(out_fd, STDOUT_FILENO);
	result = setup_redirections(cmd, data);
	if (result == -1) {
		cleanup_data(data);
		exit(1);
	}
	if (result == -2)  // Heredoc interrompu par signal
	{
		cleanup_data(data);
		exit(130);
	}
}

void	exec_child(t_data *data, t_cmd *cmd, int in_fd, int out_fd)
{
	child_setup_io_and_redirs(cmd, in_fd, out_fd, data);
	child_run_exec(data, cmd);
	cleanup_data(data);
	exit(127);
}

int	is_variable_assignment(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (str[i] == '=');
}

int	handle_variable_assignment(char *assignment, t_data *data)
{
	char	*equal_pos;
	char	*key;
	char	*value;

	equal_pos = ft_strchr(assignment, '=');
	if (!equal_pos)
		return (0);
	*equal_pos = '\0';
	key = assignment;
	value = equal_pos + 1;
	add_env_var(&data->env, key, value);
	*equal_pos = '=';
	return (1);
}

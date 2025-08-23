/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:31:13 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/15 18:44:56 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_path_directories(char **paths, char *program)
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

int	setup_pipe_fd(t_cmd *cmd, int p[2], int *out_fd)
{
	if (cmd->next)
	{
		if (pipe(p) == -1)
		{
			perror("pipe");
			return (-1);
		}
		*out_fd = p[1];
	}
	else
	{
		*out_fd = STDOUT_FILENO;
	}
	return (0);
}

void	redirect_and_close(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		exit(1);
	}
	if (old_fd != new_fd)
	{
		close(old_fd);
	}
}

void	exec_child(t_data *data, t_cmd *cmd, int in_fd, int out_fd)
{
	char	*program_path;
	char	**env_array;
	t_env	*env;

	redirect_and_close(in_fd, STDIN_FILENO);
	redirect_and_close(out_fd, STDOUT_FILENO);
	if (setup_redirections(cmd) == -1)
		exit(1);
	if (data)
		env = data->env;
	else
		env = NULL;
	program_path = find_program_path(cmd->argv[0], env);
	if (!program_path)
		program_path = ft_strdup(cmd->argv[0]);
	env_array = env_to_array(env);
	execve(program_path, cmd->argv, env_array);
	perror("execve");
	free(program_path);
	ft_free_tab(env_array);
	exit(127);
}

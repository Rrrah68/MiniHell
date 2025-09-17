/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/17 18:04:21 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_child_data(t_data *data)
{
	if (!data)
		return ;
	if (data->prompt)
	{
		free(data->prompt);
		data->prompt = NULL;
	}
	if (data->env)
	{
		free_environment(data->env);
		data->env = NULL;
	}
}

int	handle_builtin_execution(t_cmd *cmd, t_data *data)
{
	t_builtin	bi;
	int			ret;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		exit(127);
	bi = get_builtin(cmd->argv[0]);
	if (bi != BI_NONE)
	{
		ret = exec_builtin(bi, cmd->argv, data);
		exit(ret);
	}
	return (0);
}

void	child_run_exec(t_data *data, t_cmd *cmd)
{
	char		*program_path;
	char		**env_array;
	t_env		*env;
	int			has_slash;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	handle_builtin_execution(cmd, data);
	env = NULL;
	if (data)
		env = data->env;
	program_path = prepare_program_path(cmd, env, data, &has_slash);
	env_array = env_to_array(env);
	if (execve(program_path, cmd->argv, env_array) == -1)
	{
		t_exec_error_params params = {cmd->argv[0], program_path, env_array,
			data, cmd, errno};
		print_exec_error_and_exit(&params);
	}
	cleanup_child_data(data);
	exit(127);
}

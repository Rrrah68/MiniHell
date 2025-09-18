/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 18:20:50 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtin_execution(t_cmd *cmd, t_data *data)
{
	t_builtin	bi;
	int			ret;

	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		cleanup_data(data);
		exit(127);
	}
	bi = get_builtin(cmd->argv[0]);
	if (bi != BI_NONE)
	{
		ret = exec_builtin(bi, cmd->argv, data);
		cleanup_data(data);
		exit(ret);
	}
	return (0);
}

void	child_run_exec(t_data *data, t_cmd *cmd)
{
	char				*program_path;
	char				**env_array;
	t_env				*env;
	int					has_slash;
	t_exec_error_params	params;

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
		params.cmd_name = cmd->argv[0];
		params.program_path = program_path;
		params.env_array = env_array;
		params.data = data;
		params.cmd = cmd;
		params.error_type = errno;
		print_exec_error_and_exit(&params);
	}
	cleanup_data(data);
	exit(127);
}

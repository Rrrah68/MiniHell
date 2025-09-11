








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

static void	print_error_message(char *cmd_name, int error_type, char *program_path)
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

static void	cleanup_and_exit(char *program_path, char **env_array, int exit_code)
{
	if (program_path)
		free(program_path);
	if (env_array)
		ft_free_tab(env_array);
	_exit(exit_code);
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

int	handle_builtin_execution(t_cmd *cmd, t_data *data)
{
	t_builtin	bi;
	int			ret;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		_exit(127);
	bi = get_builtin(cmd->argv[0]);
	if (bi != BI_NONE)
	{
		ret = exec_builtin(bi, cmd->argv, data);
		_exit(ret);
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
	program_path = prepare_program_path(cmd, env, &has_slash);
	env_array = env_to_array(env);
	execve(program_path, cmd->argv, env_array);
	print_exec_error_and_exit(cmd->argv[0], program_path, env_array, errno);
}

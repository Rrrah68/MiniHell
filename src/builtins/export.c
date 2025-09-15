#include "minishell.h"

static int	parse_export_arg(char *arg, char **name, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		*equal_pos = '\0';
		*name = arg;
		*value = equal_pos + 1;
		return (1);
	}
	*name = arg;
	*value = NULL;
	return (0);
}

static int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_export_error(char *name)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

static int	process_export_argument(char *arg, t_data *data)
{
	char	*name;
	char	*value;
	int		had_equal;

	had_equal = parse_export_arg(arg, &name, &value);
	if (!is_valid_identifier(name))
	{
		print_export_error(name);
		if (had_equal && value)
			*(value - 1) = '=';
		data->exit_status = 1;
		return (1);
	}
	if (had_equal && value)
	{
		add_env_var(&data->env, name, value);
		*(value - 1) = '=';
	}
	else
		add_env_var(&data->env, name, "");
	return (0);
}

int	builtin_export(char **args, t_data *data)
{
	int	i;
	int	result;
	int	err;

	if (!args || !data)
		return (1);
	if (!args[1])
	{
		print_exported_vars(data->env);
		data->exit_status = 0;
		return (0);
	}
	i = 1;
	result = 0;
	while (args[i])
	{
		err = process_export_argument(args[i], data);
		if (err == 1)
			result = 1;
		i++;
	}
	data->exit_status = result;
	return (result);
}

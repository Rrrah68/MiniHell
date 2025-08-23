/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/20 19:25:12 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_exported_vars(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (current->value)
		{
			ft_printf("declare -x %s=\"%s\"\n", current->key, current->value);
		}
		else
		{
			ft_printf("declare -x %s\n", current->key);
		}
		current = current->next;
	}
}

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
	else
	{
		*name = arg;
		*value = NULL;
		return (0);
	}
}

static int	is_valid_identifier(char *name)
{
	int	i;

	i = 0;
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

static int	process_export_argument(char *arg, t_data *data)
{
	char	*name;
	char	*value;

	if (!parse_export_arg(arg, &name, &value))
		return (0);
	if (!is_valid_identifier(name))
	{
		ft_putstr_fd("export: `", STDERR_FILENO);
		ft_putstr_fd(name, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		if (value)
			*(value - 1) = '=';
		return (1);
	}
	if (value)
	{
		add_env_var(&data->env, name, value);
		*(value - 1) = '=';
	}
	else
	{
		add_env_var(&data->env, name, "");
	}
	return (0);
}

int	builtin_export(char **args, t_data *data)
{
	int	i;
	int	result;

	if (!args || !data)
		return (1);
	if (!args[1])
	{
		print_exported_vars(data->env);
		return (0);
	}
	i = 1;
	result = 0;
	while (args[i])
	{
		if (process_export_argument(args[i], data) == 1)
			result = 1;
		i++;
	}
	return (result);
}

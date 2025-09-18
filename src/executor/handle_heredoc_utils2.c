#include "minishell.h"

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

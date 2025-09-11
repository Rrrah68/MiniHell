








/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

void	init_data(t_data *data, char **envp)
{
	char	*shlvl_str;
	char	*new_shlvl_str;
	int		shlvl_value;

	data->input = NULL;
	data->lexer = NULL;
	data->prompt = NULL;
	data->env = copy_environment(envp);
	data->exit_status = 0;
	shlvl_str = get_env_value(data->env, "SHLVL");
	if (shlvl_str)
		shlvl_value = ft_atoi(shlvl_str) + 1;
	else
		shlvl_value = 1;
	new_shlvl_str = ft_itoa(shlvl_value);
	add_env_var(&data->env, "SHLVL", new_shlvl_str);
	free(new_shlvl_str);
}

char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char	*temp;
	char	*result;

	if (!s1 || !s2 || !s3)
		return (NULL);
	temp = ft_strjoin(s1, s2);
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, s3);
	free(temp);
	return (result);
}

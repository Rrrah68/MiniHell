/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:56:31 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 21:36:16 by mobullad         ###   ########.fr       */
/*                                                                            */
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
	data->cmds = NULL;
	data->prompt = NULL;
	data->env = copy_environment(envp);
	data->exit_status = 0;
	data->heredoc_interrupted = 0;
	shlvl_str = get_env_value(data->env, "SHLVL");
	if (shlvl_str)
		shlvl_value = ft_atoi(shlvl_str) + 1;
	else
		shlvl_value = 1;
	new_shlvl_str = ft_itoa(shlvl_value);
	add_env_var(&data->env, "SHLVL", new_shlvl_str);
	if (!get_env_value(data->env, "PWD"))
		add_env_var(&data->env, "PWD", getcwd(NULL, 0));
	if (!get_env_value(data->env, "OLDPWD"))
		add_env_var(&data->env, "OLDPWD", NULL);
	our_free(new_shlvl_str);
}

void	reset_heredoc_interrupt(t_data *data)
{
	data->heredoc_interrupted = 0;
}

int	is_incomplete_input(const char *input)
{
	int	len;
	int	i;

	if (!input)
		return (0);
	len = ft_strlen(input);
	if (len == 0)
		return (0);
	i = len - 1;
	while (i >= 0 && ft_isspace(input[i]))
		i--;
	if (i >= 0 && input[i] == '|')
		return (1);
	return (0);
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
	our_free(temp);
	return (result);
}

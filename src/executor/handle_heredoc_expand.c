/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc_expand.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 16:34:39 by radahman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_to_result(char *result, char *temp)
{
	char	*new_result;

	new_result = ft_strjoin(result, temp);
	our_free(result);
	our_free(temp);
	return (new_result);
}

char	*process_character(const char *line, int *i, t_data *data)
{
	if (line[*i] == '$' && line[*i + 1]
		&& (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_'))
		return (expand_var_in_heredoc(line, *i, i, data));
	else
	{
		(*i)++;
		return (ft_substr(line, *i - 1, 1));
	}
}

char	*expand_var_in_heredoc(const char *line, int i, int *new_i,
		t_data *data)
{
	int		j;
	char	*temp;
	char	*var_value;

	j = i + 1;
	while (line[j] && (ft_isalnum(line[j]) || line[j] == '_'))
		j++;
	temp = ft_substr(line, i + 1, j - i - 1);
	if (!temp)
		return (NULL);
	var_value = get_env_value(data->env, temp);
	our_free(temp);
	*new_i = j;
	if (!var_value)
		return (ft_strdup(""));
	return (ft_strdup(var_value));
}

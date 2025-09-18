/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:50:32 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 16:34:39 by radahman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*handle_exit_status(t_token *dollar, t_token *next, t_data *data)
{
	char	*var_value;

	var_value = ft_itoa(data->exit_status);
	replace_and_remove_next(dollar, next, var_value);
	our_free(var_value);
	return (dollar->next);
}

t_token	*handle_double_dollar(t_token *dollar, t_token *next)
{
	replace_and_remove_next(dollar, next, "");
	return (dollar->next);
}

void	handle_partial_expansion(t_token *dollar, t_token *next,
		char *var_value, char *var_name)
{
	if (var_value)
		replace_with_expansion(dollar, next, var_value, var_name);
	else
		replace_with_expansion(dollar, next, "", var_name);
}

int	find_valid_var_length(char *str, t_data *data)
{
	char	*full_name;
	int		len;

	if (!str)
		return (0);
	if (ft_isdigit(str[0]))
	{
		return (1);
	}
	full_name = extract_var_from_word(str);
	if (!full_name)
		return (0);
	len = ft_strlen(full_name);
	if (get_env_value(data->env, full_name))
	{
		our_free(full_name);
		return (len);
	}
	our_free(full_name);
	return (len);
}

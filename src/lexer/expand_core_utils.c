/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_core_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:54:55 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/16 15:54:56 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*handle_dollar_not_found(t_token **dollar)
{
	if (dollar && *dollar)
	{
		(*dollar)->type = WORD;
		(*dollar)->str = ft_strdup("$");
		return ((*dollar)->next);
	}
	return (NULL);
}

t_token	*handle_invalid_word_token(t_token **dollar)
{
	(*dollar)->type = WORD;
	(*dollar)->str = ft_strdup("$");
	return ((*dollar)->next);
}

t_token	*process_variable_expansion(t_token **dollar, t_token *word_token,
		t_data *data, int var_len)
{
	char	*var_name;
	char	*var_value;

	var_name = ft_substr(word_token->str, 0, var_len);
	var_value = get_env_value(data->env, var_name);
	if (!var_value)
		var_value = "";
	perform_variable_expansion(*dollar, word_token, var_name, var_value);
	free(var_name);
	return ((*dollar)->next);
}

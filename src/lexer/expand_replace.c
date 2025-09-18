/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_replace.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:56:02 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 16:34:39 by radahman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_variable_tokens(t_token *dollar, char *var_name,
		char *new_content)
{
	t_token		*current;
	t_token		*to_remove;
	int			var_len;
	int			i;

	var_len = ft_strlen(var_name);
	current = dollar->next;
	i = 0;
	while (current && i < var_len)
	{
		to_remove = current;
		current = current->next;
		if (to_remove->str)
			our_free(to_remove->str);
		our_free(to_remove);
		i++;
	}
	dollar->next = current;
	if (dollar->str)
		our_free(dollar->str);
	dollar->str = new_content;
	dollar->type = WORD;
	dollar->c = 0;
}

void	replace_variable_tokens_partial(t_token *dollar, char *var_name,
		char *new_content)
{
	t_token		*word_token;
	char		*remaining;
	char		*final_result;
	int			var_len;

	var_len = ft_strlen(var_name);
	word_token = dollar->next;
	if (word_token && word_token->type == WORD && word_token->str)
	{
		remaining = ft_strdup(word_token->str + var_len);
		final_result = ft_strjoin(new_content, remaining);
		our_free(remaining);
		our_free(new_content);
		new_content = final_result;
		dollar->next = word_token->next;
		our_free(word_token->str);
		our_free(word_token);
	}
	if (dollar->str)
		our_free(dollar->str);
	dollar->str = new_content;
	dollar->type = WORD;
	dollar->c = 0;
}

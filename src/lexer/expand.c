/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:54:44 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 16:34:39 by radahman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_with_expansion(t_token *dollar, t_token *word, char *value,
		char *var_name)
{
	char	*remaining;
	char	*new_content;

	if (!dollar || !word || !var_name)
		return ;
	remaining = word->str + ft_strlen(var_name);
	if (value)
		new_content = ft_strjoin(value, remaining);
	else
		new_content = ft_strdup(remaining);
	if (dollar->str)
		our_free(dollar->str);
	dollar->str = new_content;
	dollar->type = WORD;
	dollar->c = 0;
	dollar->next = word->next;
	if (word->str)
		our_free(word->str);
	our_free(word);
}

void	replace_and_remove_next(t_token *current, t_token *to_remove,
		char *new_value)
{
	char	*old_content;
	char	*new_content;

	if (!current || !to_remove)
		return ;
	if (current->str && ft_strlen(current->str) > 0)
		old_content = current->str;
	else
		old_content = "";
	if (new_value && ft_strlen(new_value) > 0)
		new_content = ft_strjoin(old_content, new_value);
	else
		new_content = ft_strdup(old_content);
	if (current->str)
		our_free(current->str);
	current->str = new_content;
	current->type = WORD;
	current->c = 0;
	current->next = to_remove->next;
	if (to_remove->str)
		our_free(to_remove->str);
	our_free(to_remove);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_merge_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/22 18:33:13 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* verifie s'il y a un separateur entre deux tokens */
static int	is_separator_between(t_token *current, t_token *next)
{
	t_token	*temp;

	temp = current->next;
	while (temp && temp != next)
	{
		if (temp->type == WHITESPACE)
			return (1);
		temp = temp->next;
	}
	return (0);
}

/* fusionne deux tokens word adjacents */
static void	merge_two_tokens(t_token *current, t_token *next)
{
	char	*merged_str;

	if (!current->str)
		current->str = ft_strdup("");
	if (!next->str)
		next->str = ft_strdup("");
	merged_str = ft_strjoin(current->str, next->str);
	free(current->str);
	current->str = merged_str;
	current->next = next->next;
	free(next->str);
	free(next);
}

/* fusionne les tokens word adjacents (sauf si separes par expansion) */
void	merge_adjacent_word_tokens(t_data *data)
{
	t_token	*current;
	t_token	*next;

	if (!data || !data->lexer)
		return ;
	current = data->lexer;
	while (current && current->next)
	{
		next = current->next;
		if (current->type == WORD && next->type == WORD
			&& !is_separator_between(current, next) && !current->from_expansion
			&& !next->from_expansion)
		{
			merge_two_tokens(current, next);
		}
		else
			current = current->next;
	}
}

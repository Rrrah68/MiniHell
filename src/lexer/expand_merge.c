/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_merge.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:55:58 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 16:34:39 by radahman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_token_replacement(t_token **lexer, t_token *current,
		t_token *prev, t_token *new_tokens)
{
	t_token	*last_new;

	last_new = new_tokens;
	while (last_new->next)
		last_new = last_new->next;
	if (prev)
		prev->next = new_tokens;
	else
		*lexer = new_tokens;
	last_new->next = current->next;
	our_free(current->str);
	our_free(current);
}

static void	handle_word_with_spaces(t_data *data, t_token **current,
		t_token **prev)
{
	t_token	*new_tokens;

	new_tokens = split_word_on_spaces((*current)->str);
	if (new_tokens)
	{
		process_token_replacement(&data->lexer, *current, *prev, new_tokens);
		*current = data->lexer;
		*prev = NULL;
	}
	else
	{
		*prev = *current;
		*current = (*current)->next;
	}
}

static void	process_word_splitting(t_data *data, t_token **current,
		t_token **prev)
{
	if ((*current)->type == WORD && (*current)->str
		&& ft_strchr((*current)->str, ' ') && !(*current)->in_quotes)
		handle_word_with_spaces(data, current, prev);
	else
	{
		*prev = *current;
		*current = (*current)->next;
	}
}

void	merge_adjacent_words(t_data *data)
{
	t_token	*current;
	t_token	*prev;

	if (!data || !data->lexer)
		return ;
	current = data->lexer;
	prev = NULL;
	while (current)
		process_word_splitting(data, &current, &prev);
}

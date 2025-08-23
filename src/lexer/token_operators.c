/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:26:45 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/22 18:33:13 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* supprime les tokens de guillemets apres traitement */
void	remove_quote_token(t_data *data)
{
	t_token	*current;
	t_token	*prev;
	t_token	*to_remove;

	if (!data || !data->lexer)
		return ;
	current = data->lexer;
	prev = NULL;
	while (current)
	{
		if (current->type == SINGLE_QUOTE || current->type == DOUBLE_QUOTE)
		{
			to_remove = current;
			current = current->next;
			remove_quote_from_list(data, to_remove, prev);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

/* traite les operateurs de redirection doubles */
void	handle_operators(t_data *data)
{
	t_token	*current;

	if (!data || !data->lexer)
		return ;
	current = data->lexer;
	while (current && current->next)
	{
		if (current->c == '>' && current->next->c == '>'
			&& current->type == SYMBOL && current->next->type == SYMBOL)
			handle_append_redirect(current);
		else if (current->c == '<' && current->next->c == '<'
			&& current->type == SYMBOL && current->next->type == SYMBOL)
			handle_heredoc_redirect(current);
		else
			current = current->next;
	}
}

/* traite les operateurs simples (pipe, redirection) */
void	handle_single_operators(t_data *data)
{
	t_token	*current;

	if (!data || !data->lexer)
		return ;
	current = data->lexer;
	while (current)
	{
		handle_single_operator_type(current);
		current = current->next;
	}
}

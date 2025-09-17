/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_single_quote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:56:06 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/16 15:56:07 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*handle_single_quote(t_token *quote_start)
{
	t_token	*current;

	if (!quote_start)
		return (NULL);
	current = quote_start->next;
	while (current && !(current->c == '\'' && current->type == SYMBOL))
	{
		current->type = CHAR;
		current->in_quotes = 1;
		current = current->next;
	}
	if (current && current->c == '\'')
	{
		current->type = SINGLE_QUOTE;
		if (current->next)
			return (current->next);
		return (current);
	}
	ft_putstr_fd("Error: Unclosed single quote\n", 2);
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_single_quote.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:41:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/22 17:41:00 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* gere le contenu entre guillemets simples */
t_token	*handle_single_quote(t_token *quote_start)
{
	t_token	*current;

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
		else
			return (current);
	}
	else if (!current)
	{
		ft_putstr_fd("Error: Unclosed single quote\n", 2);
		return (NULL);
	}
	return (current);
}

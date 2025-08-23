/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:40:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/22 18:33:13 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* marque le contenu entre guillemets doubles */
static void	mark_quote_content(t_token *current)
{
	while (current && !(current->c == '"' && current->type == SYMBOL))
	{
		if (current->c != '$' && current->c != '?')
		{
			current->type = CHAR;
			current->in_quotes = 1;
		}
		current = current->next;
	}
}

/* trouve et marque le guillemet de fermeture */
static t_token	*find_closing_double_quote(t_token *current)
{
	if (current && current->c == '"')
	{
		current->type = DOUBLE_QUOTE;
		if (current->next)
			return (current->next);
		else
			return (current);
	}
	else if (!current)
	{
		ft_putstr_fd("Error: Unclosed double quote\n", 2);
		return (NULL);
	}
	return (current);
}

/* gere le contenu entre guillemets doubles */
t_token	*handle_double_quote(t_token *quote_start)
{
	t_token	*current;

	current = quote_start->next;
	mark_quote_content(current);
	while (current && !(current->c == '"' && current->type == SYMBOL))
		current = current->next;
	return (find_closing_double_quote(current));
}

/* gere l'expansion des variables numeriques ($0, $1, etc) */
t_token	*handle_numeric_variable(t_token *dollar, t_token *next, t_data *data)
{
	char	*var_value;
	char	*final_value;
	char	*remaining;

	(void)data;
	if (next->c == '0')
		var_value = "minishell";
	else
		var_value = "";
	remaining = ft_strdup(next->str + 1);
	final_value = ft_strjoin(var_value, remaining);
	free(dollar->str);
	dollar->str = final_value;
	dollar->type = WORD;
	dollar->c = 0;
	dollar->next = next->next;
	free(next->str);
	free(next);
	free(remaining);
	return (dollar->next);
}

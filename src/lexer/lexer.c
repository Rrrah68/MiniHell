/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:54:16 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/22 18:44:16 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* execute toutes les phases de traitement du lexer */
static void	lexer_process(t_data *data)
{
	if (!process_tokens_quotes(data))
	{
		free_tokens(data->lexer);
		data->lexer = NULL;
		return ;
	}
	handle_operators(data);
	handle_single_operators(data);
	merge_double_quoted_content(data->lexer);
	merge_single_quoted_content(data->lexer);
	merge_char_tokens(data->lexer);
	expand_var(data);
	remove_quote_token(data);
	merge_adjacent_word_tokens(data);
}

/* tokenise l'input caractere par caractere */
void	lexer(t_data *data, char *input)
{
	t_token	*new_token;
	int		i;

	i = 0;
	new_token = NULL;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			new_token = create_token(input[i], WHITESPACE);
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>'
			|| input[i] == '$' || input[i] == '"' || input[i] == '\''
			|| input[i] == '?')
			new_token = create_token(input[i], SYMBOL);
		else
			new_token = create_token(input[i], CHAR);
		if (!new_token)
		{
			free_tokens(data->lexer);
			return ;
		}
		add_token(&data->lexer, new_token);
		i++;
	}
	lexer_process(data);
}

/* traite les guillemets et marque leur contenu */
int	process_tokens_quotes(t_data *data)
{
	t_token	*current;

	if (!data || !data->lexer)
		return (1);
	current = data->lexer;
	while (current)
	{
		if (current->c == '\'' && current->type == SYMBOL)
		{
			current->type = SINGLE_QUOTE;
			current = handle_single_quote(current);
			if (!current)
				return (0);
		}
		else if (current->c == '"' && current->type == SYMBOL)
		{
			current->type = DOUBLE_QUOTE;
			current = handle_double_quote(current);
			if (!current)
				return (0);
		}
		else
			current = current->next;
	}
	return (1);
}

/* affiche tous les tokens du lexer pour le debug */
void	print_lexer(t_token *lexer)
{
	t_token	*current;

	current = lexer;
	printf("=== LEXER TOKENS ===\n");
	while (current)
	{
		if (current->str)
			printf("Token: '%s', Type: %s\n", current->str,
				get_type(current->type));
		else
			printf("Token: '%c', Type: %s\n", current->c,
				get_type(current->type));
		current = current->next;
	}
}

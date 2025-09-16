/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:54:51 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/16 15:54:52 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_var(t_data *data)
{
	int	found_expansion;

	if (!data || !data->lexer)
		return ;
	found_expansion = 1;
	while (found_expansion)
	{
		found_expansion = process_expansion_loop(data);
	}
	merge_adjacent_words(data);
}

static t_token	*handle_quote_tokens(t_token *current, int *in_single_quotes,
		int *in_double_quotes)
{
	if (current->type == SINGLE_QUOTE)
	{
		*in_single_quotes = !(*in_single_quotes);
		return (current->next);
	}
	else if (current->type == DOUBLE_QUOTE)
	{
		*in_double_quotes = !(*in_double_quotes);
		return (current->next);
	}
	return (NULL);
}

static t_token	*handle_expansion_tokens(t_token *current, t_data *data,
		int in_single_quotes, int in_double_quotes)
{
	if (current->c == '$' && current->type == SYMBOL && !in_single_quotes)
	{
		if (!current->next)
		{
			current->type = WORD;
			current->str = ft_strdup("$");
			return (current);
		}
		if (current->next->type == DOUBLE_QUOTE
			|| current->next->type == SINGLE_QUOTE)
			return (NULL);
		if (in_double_quotes)
			current->in_quotes = 1;
		return (expand_single_variable(&current, data));
	}
	else if (current->type == WORD && current->str && !in_single_quotes
		&& ft_strchr(current->str, '$'))
	{
		if (in_double_quotes)
			current->in_quotes = 1;
		return (expand_word_variable(current, data));
	}
	return (NULL);
}

int	process_expansion_loop(t_data *data)
{
	t_token	*current;
	t_token	*result;
	int		in_single_quotes;
	int		in_double_quotes;

	current = data->lexer;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (current)
	{
		result = handle_quote_tokens(current, &in_single_quotes,
				&in_double_quotes);
		if (result)
			current = result;
		else
		{
			result = handle_expansion_tokens(current, data, in_single_quotes,
					in_double_quotes);
			if (result)
				return (1);
			else
				current = current->next;
		}
	}
	return (0);
}

t_token	*expand_single_variable(t_token **dollar, t_data *data)
{
	t_token	*word_token;
	int		var_len;

	if (!dollar || !*dollar || !(*dollar)->next)
		return (handle_dollar_not_found(dollar));
	if (check_special_cases(*dollar, data))
		return ((*dollar)->next);
	word_token = (*dollar)->next;
	if (!word_token || word_token->type != WORD || !word_token->str)
		return (handle_invalid_word_token(dollar));
	var_len = find_valid_var_length(word_token->str, data);
	if (var_len == 0)
		return (handle_invalid_word_token(dollar));
	return (process_variable_expansion(dollar, word_token, data, var_len));
}

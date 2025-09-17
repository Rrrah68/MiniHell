/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:56:11 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/16 15:56:12 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*handle_word_creation(t_token *start_quote, t_token *end_quote,
		char *content)
{
	t_token	*word_token;

	word_token = start_quote->next;
	if (word_token && word_token != end_quote)
	{
		setup_existing_word_token(word_token, content, end_quote);
		start_quote->next = word_token;
	}
	else
	{
		word_token = create_word_token_with_content(content, end_quote);
		if (!word_token)
			return (NULL);
		start_quote->next = word_token;
	}
	return (end_quote->next);
}

static t_token	*process_quoted_section(t_token *start_quote)
{
	t_token	*end_quote;
	char	*content;

	end_quote = find_closing_quote(start_quote->next);
	if (!end_quote)
		return (NULL);
	content = build_quoted_content(start_quote->next, end_quote);
	if (!content)
		content = ft_strdup("");
	if (!content)
		return (NULL);
	return (handle_word_creation(start_quote, end_quote, content));
}

static t_token	*process_single_quoted_section(t_token *start_quote)
{
	t_token	*end_quote;
	char	*content;

	end_quote = find_closing_single_quote(start_quote->next);
	if (!end_quote)
		return (NULL);
	content = build_quoted_content(start_quote->next, end_quote);
	if (!content)
		content = ft_strdup("");
	if (!content)
		return (NULL);
	return (handle_word_creation(start_quote, end_quote, content));
}

void	merge_double_quoted_content(t_token *lexer)
{
	t_token	*current;
	t_token	*next;

	if (!lexer)
		return ;
	current = lexer;
	while (current)
	{
		if (current->type == DOUBLE_QUOTE)
		{
			next = process_quoted_section(current);
			if (!next)
				break ;
			current = next;
		}
		else
			current = current->next;
	}
}

void	merge_single_quoted_content(t_token *lexer)
{
	t_token	*current;
	t_token	*next;

	if (!lexer)
		return ;
	current = lexer;
	while (current)
	{
		if (current->type == SINGLE_QUOTE)
		{
			next = process_single_quoted_section(current);
			if (!next)
				break ;
			current = next;
		}
		else
			current = current->next;
	}
}

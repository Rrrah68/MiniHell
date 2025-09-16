/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:56:15 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/16 15:56:16 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_existing_word_token(t_token *word_token, char *content,
		t_token *end_quote)
{
	word_token->str = content;
	word_token->type = WORD;
	word_token->c = 0;
	cleanup_intermediate_tokens(word_token, end_quote);
	word_token->next = end_quote;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:56:18 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 16:34:39 by radahman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_merge_char_tokens(t_token *start)
{
	t_token	*next;
	char	*word;
	char	*temp;
	char	char_str[2];

	char_str[0] = start->c;
	char_str[1] = '\0';
	word = ft_strdup(char_str);
	if (!word)
		return (NULL);
	next = start->next;
	while (next && next->type == CHAR)
	{
		char_str[0] = next->c;
		temp = word;
		word = ft_strjoin(word, char_str);
		our_free(temp);
		if (!word)
			return (NULL);
		start->next = next->next;
		our_free(next);
		next = start->next;
	}
	return (word);
}

void	handle_space_token(t_token **current, t_token **prev)
{
	t_token	*temp;

	if (*prev)
		(*prev)->next = (*current)->next;
	temp = *current;
	*current = (*current)->next;
	our_free(temp);
}

void	handle_char_token(t_token **current, t_token **prev)
{
	char	*word;

	word = process_merge_char_tokens(*current);
	if (!word)
		return ;
	(*current)->str = word;
	(*current)->type = WORD;
	*prev = *current;
	*current = (*current)->next;
}

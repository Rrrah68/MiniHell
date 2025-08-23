/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:53:31 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/22 18:33:13 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* cree un nouveau token avec un caractere et un type */
t_token	*create_token(char c, t_token_type type)
{
	t_token	*new_token;

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->c = c;
	new_token->type = type;
	new_token->in_quotes = 0;
	new_token->next = NULL;
	return (new_token);
}

/* ajoute un token a la fin de la liste chainee */
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		new_token->prev = NULL;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
	new_token->prev = current;
}

/* fusionne les tokens char consecutifs en mots */
void	merge_char_tokens(t_token *lexer)
{
	t_token	*current;
	t_token	*prev;

	if (!lexer)
		return ;
	current = lexer;
	prev = NULL;
	while (current)
	{
		if (current->type == CHAR && (current->c == ' ' || current->c == '\t')
			&& !current->in_quotes)
			handle_space_token(&current, &prev);
		else if (current->type == CHAR)
			handle_char_token(&current, &prev);
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

/* libere la memoire de tous les tokens */
void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->str)
			free(tmp->str);
		free(tmp);
	}
}

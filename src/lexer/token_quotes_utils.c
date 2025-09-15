#include "minishell.h"

char	*build_quoted_content(t_token *start, t_token *end)
{
	t_token	*current;
	char	*content;
	char	*temp;
	char	char_str[2];

	content = ft_strdup("");
	if (!content)
		return (NULL);
	current = start;
	while (current && current != end)
	{
		if (current->type == CHAR || current->type == SYMBOL
			|| current->type == WHITESPACE)
		{
			char_str[0] = current->c;
			char_str[1] = '\0';
			temp = content;
			content = ft_strjoin(content, char_str);
			free(temp);
			if (!content)
				return (NULL);
		}
		current = current->next;
	}
	return (content);
}

void	cleanup_intermediate_tokens(t_token *start, t_token *end)
{
	t_token	*to_delete;
	t_token	*next;

	to_delete = start->next;
	while (to_delete && to_delete != end)
	{
		next = to_delete->next;
		if (to_delete->str)
			free(to_delete->str);
		free(to_delete);
		to_delete = next;
	}
	start->next = end;
}

t_token	*find_closing_quote(t_token *start)
{
	t_token	*current;

	current = start;
	while (current && current->type != DOUBLE_QUOTE)
		current = current->next;
	return (current);
}

t_token	*find_closing_single_quote(t_token *start)
{
	t_token	*current;

	current = start;
	while (current && current->type != SINGLE_QUOTE)
		current = current->next;
	return (current);
}

t_token	*create_word_token_with_content(char *content, t_token *end_quote)
{
	t_token	*word_token;

	word_token = create_token(0, WORD);
	if (!word_token)
	{
		free(content);
		return (NULL);
	}
	word_token->str = content;
	word_token->next = end_quote;
	return (word_token);
}

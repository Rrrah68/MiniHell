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

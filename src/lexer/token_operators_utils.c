








/* ************************************************************************** */

#include "minishell.h"

void	remove_next_token(t_token *current)
{
	t_token	*to_remove;

	if (!current || !current->next)
		return ;
	to_remove = current->next;
	current->next = to_remove->next;
	if (to_remove && to_remove->str)
		free(to_remove->str);
	free(to_remove);
}

void	remove_quote_from_list(t_data *data, t_token *to_remove,
		t_token *prev)
{
	if (prev)
		prev->next = to_remove->next;
	else
		data->lexer = to_remove->next;
	if (to_remove->str)
		free(to_remove->str);
	free(to_remove);
}

void	handle_append_redirect(t_token *current)
{
	current->str = ft_strdup(">>");
	current->type = REDIRECT_APPEND;
	current->c = 0;
	remove_next_token(current);
}

void	handle_heredoc_redirect(t_token *current)
{
	current->str = ft_strdup("<<");
	current->type = REDIRECT_HEREDOC;
	current->c = 0;
	remove_next_token(current);
}

void	handle_single_operator_type(t_token *current)
{
	if (current->c == '|' && current->type == SYMBOL)
	{
		current->type = PIPE;
		current->str = ft_strdup("|");
		current->c = 0;
	}
	else if (current->c == '<' && current->type == SYMBOL)
	{
		current->type = REDIRECT_IN;
		current->str = ft_strdup("<");
		current->c = 0;
	}
	else if (current->c == '>' && current->type == SYMBOL)
	{
		current->type = REDIRECT_OUT;
		current->str = ft_strdup(">");
		current->c = 0;
	}
}

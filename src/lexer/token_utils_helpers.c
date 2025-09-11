








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
		free(temp);
		if (!word)
			return (NULL);
		start->next = next->next;
		free(next);
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
	free(temp);
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

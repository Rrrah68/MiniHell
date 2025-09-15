#include "minishell.h"

t_token	*create_word_token(char *word_str)
{
	t_token	*new_token;

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(word_str);
	new_token->type = WORD;
	new_token->in_quotes = 0;
	new_token->from_expansion = 1;
	return (new_token);
}

static t_token	*create_token_list(char **words)
{
	t_token	*head;
	t_token	*new_token;
	t_token	*current;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (words[i])
	{
		new_token = create_word_token(words[i]);
		if (!new_token)
			return (NULL);
		if (!head)
			head = new_token;
		else
			current->next = new_token;
		current = new_token;
		i++;
	}
	return (head);
}

t_token	*split_word_on_spaces(char *str)
{
	char	**words;
	t_token	*head;

	words = ft_split(str, ' ');
	if (!words || !words[0])
		return (NULL);
	head = create_token_list(words);
	ft_free_tab(words);
	return (head);
}

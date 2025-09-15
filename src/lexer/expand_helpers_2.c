#include "minishell.h"

void	handle_quote_removal(t_token *word)
{
	char	*new_str;

	new_str = ft_substr(word->str, 1, ft_strlen(word->str) - 2);
	free(word->str);
	word->str = new_str;
}

void	remove_double_dollars(t_token *word)
{
	int		i;
	char	*new_str;

	i = 0;
	while (word->str[i])
	{
		if (word->str[i] == '$' && word->str[i + 1] && word->str[i + 1] == '$')
		{
			i += 2;
			new_str = ft_substr(word->str, 0, i - 2);
			new_str = ft_strjoin_f1(new_str, word->str + i);
			free(word->str);
			word->str = new_str;
			i = 0;
		}
		else
			i++;
	}
}

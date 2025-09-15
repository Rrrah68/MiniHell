#include "minishell.h"

static char	*build_expanded_word(char *word_str, char *dollar_pos,
		char *var_value, int var_len)
{
	char	*before;
	char	*after;
	char	*temp;
	char	*result;

	before = ft_substr(word_str, 0, dollar_pos - word_str);
	after = ft_strdup(dollar_pos + 1 + var_len);
	temp = ft_strjoin(before, var_value);
	result = ft_strjoin(temp, after);
	free(before);
	free(after);
	free(temp);
	return (result);
}

t_token	*delete_dollars(t_token *word, char *dollar_pos)
{
	if (!word || !dollar_pos)
		return (NULL);
	if (dollar_pos[1] && dollar_pos[1] == '"')
	{
		if (word->in_quotes)
			return (word);
		handle_quote_removal(word);
	}
	remove_double_dollars(word);
	return (word);
}

t_token	*expand_word_variable(t_token *word, t_data *data)
{
	char	*dollar_pos;
	char	*var_value;
	char	*new_str;
	int		var_len;

	dollar_pos = ft_strchr(word->str, '$');
	if (!dollar_pos)
		return (word->next);
	var_value = get_variable_value(dollar_pos, &var_len, data);
	if (var_len == -1)
		return (delete_dollars(word, dollar_pos));
	if (!var_value || var_len == 0)
		return (NULL);
	new_str = build_expanded_word(word->str, dollar_pos, var_value, var_len);
	if (!update_word_content(word, new_str))
		return (NULL);
	if (ft_strchr(word->str, '$'))
		return (word);
	return (word->next);
}

int	check_special_cases(t_token *dollar, t_data *data)
{
	if ((dollar->next->c == '?' && dollar->next->type == SYMBOL)
		|| (dollar->next->c == '?' && dollar->next->type == CHAR))
	{
		handle_exit_status(dollar, dollar->next, data);
		return (1);
	}
	if ((dollar->next->c == '$' && dollar->next->type == SYMBOL)
		|| (dollar->next->c == '$' && dollar->next->type == CHAR))
	{
		handle_double_dollar(dollar, dollar->next);
		return (1);
	}
	if (ft_isdigit(dollar->next->c) && (dollar->next->type == CHAR
			|| dollar->next->type == SYMBOL || dollar->next->type == WORD))
	{
		handle_numeric_variable(dollar, dollar->next, data);
		return (1);
	}
	return (0);
}

void	perform_variable_expansion(t_token *dollar, t_token *word_token,
		char *var_name, char *var_value)
{
	char	*remaining;
	char	*result;

	remaining = ft_strdup(word_token->str + ft_strlen(var_name));
	result = ft_strjoin(var_value, remaining);
	if (dollar->str)
		free(dollar->str);
	dollar->str = result;
	dollar->type = WORD;
	dollar->c = 0;
	dollar->next = word_token->next;
	free(word_token->str);
	free(word_token);
	free(remaining);
}










/* ************************************************************************** */

#include "minishell.h"

char	*collect_var_name(t_token *start)
{
	char	buffer[256];
	int		i;
	t_token	*current;

	if (!start)
		return (NULL);
	i = 0;
	current = start;
	if (current->type == WORD && current->str)
		return (extract_var_from_word(current->str));
	if (current->type == CHAR && (ft_isalpha(current->c) || current->c == '_'))
	{
		while (current && current->type == CHAR && (ft_isalnum(current->c)
				|| current->c == '_') && i < 255)
		{
			buffer[i++] = current->c;
			current = current->next;
		}
		buffer[i] = '\0';
		return (ft_strdup(buffer));
	}
	return (NULL);
}

char	*extract_var_from_word(char *str)
{
	int	i;

	if (!str)
		return (NULL);
	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (NULL);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, 0, i));
}

t_token	*find_previous_token(t_token *head, t_token *target)
{
	t_token	*current;

	if (!head || !target || head == target)
		return (NULL);
	current = head;
	while (current && current->next)
	{
		if (current->next == target)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static char	*handle_special_vars(char *var_start, int *var_len, t_data *data)
{
	if (var_start && (var_start[0] == '$' || var_start[0] == '"'))
		return (*var_len = -1, NULL);
	if (var_start[0] == '?')
	{
		*var_len = 1;
		return (ft_itoa(data->exit_status));
	}
	if (ft_isdigit(var_start[0]))
	{
		*var_len = 1;
		if (var_start[0] == '0')
			return (ft_strdup("minishell"));
		else
			return (ft_strdup(""));
	}
	return (NULL);
}

char	*get_variable_value(char *dollar_pos, int *var_len, t_data *data)
{
	char	*var_name;
	char	*var_start;
	char	*value;
	char	*special_val;

	var_start = dollar_pos + 1;
	special_val = handle_special_vars(var_start, var_len, data);
	if (special_val || *var_len == -1)
		return (special_val);
	*var_len = find_valid_var_length(var_start, data);
	if (*var_len == 0)
		return (NULL);
	var_name = ft_substr(var_start, 0, *var_len);
	value = get_env_value(data->env, var_name);
	free(var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

int	update_word_content(t_token *word, char *new_str)
{
	if (!new_str)
		return (0);
	free(word->str);
	word->str = new_str;
	return (1);
}

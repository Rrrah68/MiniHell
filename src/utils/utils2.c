#include "minishell.h"

char	*get_type(t_token_type type)
{
	if (type == CHAR)
		return ("CHAR");
	else if (type == SYMBOL)
		return ("SYMBOL");
	else if (type == WHITESPACE)
		return ("WHITESPACE");
	else if (type == SINGLE_QUOTE)
		return ("SIMPLE_QUOTE");
	else if (type == DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	else if (type == WORD)
		return ("WORD");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == REDIRECT_IN)
		return ("REDIRECT_IN");
	else if (type == REDIRECT_OUT)
		return ("REDIRECT_OUT");
	else if (type == REDIRECT_APPEND)
		return ("REDIRECT_APPEND");
	else if (type == REDIRECT_HEREDOC)
		return ("REDIRECT_HEREDOC");
	return ("UNKNOWN");
}

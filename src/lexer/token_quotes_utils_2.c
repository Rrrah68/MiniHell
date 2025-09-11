








/* ************************************************************************** */

#include "minishell.h"

void	setup_existing_word_token(t_token *word_token, char *content,
		t_token *end_quote)
{
	word_token->str = content;
	word_token->type = WORD;
	word_token->c = 0;
	cleanup_intermediate_tokens(word_token, end_quote);
	word_token->next = end_quote;
}

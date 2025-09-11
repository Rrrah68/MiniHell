








/* ************************************************************************** */

#include "minishell.h"

int	handle_redir(t_token **tok, t_cmd *cur)
{
	t_token	*op;
	t_token	*file;

	op = *tok;
	if (!op)
		return (0);
	if (op->in_quotes == 1)
		return (0);
	if (op->type != REDIRECT_IN && op->type != REDIRECT_OUT
		&& op->type != REDIRECT_APPEND && op->type != REDIRECT_HEREDOC)
		return (0);
	file = op->next;
	while (file && file->type == WHITESPACE)
		file = file->next;
	if (!file || file->type != WORD || !file->str)
		return (0);
	if (!cmd_add_redir(cur, op->type, file->str))
		return (0);
	*tok = file;
	return (1);
}

int	handle_pipe(t_cmd **cmds, t_cmd **cur, t_token *tok)
{
	if (!tok || !tok->next || tok->next->type == PIPE)
		return (0);
	*cur = create_cmd();
	if (!*cur)
		return (-1);
	add_cmd(cmds, *cur);
	return (1);
}
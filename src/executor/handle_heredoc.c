








/* ************************************************************************** */

#include "minishell.h"

char	*expand_heredoc_line(const char *line, t_data *data)
{
	char	*result;
	char	*temp;
	int		i;
	int		j;

	if (!line || !data)
		return (ft_strdup(line));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] 
			&& (ft_isalnum(line[i + 1]) || line[i + 1] == '_'))
		{
			j = i + 1;
			while (line[j] && (ft_isalnum(line[j]) || line[j] == '_'))
				j++;
			temp = ft_substr(line, i + 1, j - i - 1);
			if (temp)
			{
				char *var_value = get_env_value(data->env, temp);
				char *new_result = ft_strjoin(result, var_value ? var_value : "");
				free(result);
				free(temp);
				result = new_result;
				i = j;
			}
			else
				i++;
		}
		else
		{
			temp = ft_substr(line, i, 1);
			char *new_result = ft_strjoin(result, temp);
			free(result);
			free(temp);
			result = new_result;
			i++;
		}
	}
	return (result);
}

static void	process_heredoc_line(char *line, int quoted, t_data *data, 
				int write_fd)
{
	char	*expanded;

	if (!quoted)
	{
		expanded = expand_heredoc_line(line, data);
		write_heredoc_line(write_fd, expanded);
		free(expanded);
	}
	else
		write_heredoc_line(write_fd, line);
}

int	handle_heredoc(const char *delimiter, int quoted, t_data *data)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = read_heredoc_line_input();
		if (!line)
		{
			break ;
		}
		if (is_delimiter(line, delimiter))
		{
			free(line);
			break ;
		}
		process_heredoc_line(line, quoted, data, pipefd[1]);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

int setup_heredoc(t_cmd *cmd, t_data *data)
{
	if (!cmd->heredoc_limiter)
		return (0);
	if (cmd->heredoc_content)
	{
		cmd->heredoc_fd = handle_heredoc_with_content(cmd->heredoc_content);
	}
	else
	{
		cmd->heredoc_fd = handle_heredoc(cmd->heredoc_limiter, 
			cmd->heredoc_quoted, data);
	}
	if (cmd->heredoc_fd == -1)
		return (-1);
	if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 heredoc");
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
		return (-1);
	}
	close(cmd->heredoc_fd);
	cmd->heredoc_fd = -1;
	return (0);
}

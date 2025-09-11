








/* ************************************************************************** */

#include "minishell.h"

static void	process_escape_sequences(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
		{
			i++;
			if (str[i] == 'n')
				str[j++] = '\n';
			else if (str[i] == 't')
				str[j++] = '\t';
			else if (str[i] == 'r')
				str[j++] = '\r';
			else if (str[i] == 'b')
				str[j++] = '\b';
			else if (str[i] == 'f')
				str[j++] = '\f';
			else if (str[i] == 'v')
				str[j++] = '\v';
			else if (str[i] == 'a')
				str[j++] = '\a';
			else if (str[i] == '\\')
				str[j++] = '\\';
			else
			{
				str[j++] = '\\';
				str[j++] = str[i];
			}
		}
		else
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
}

int	builtin_echo(char **argv)
{
	int	i;
	int	newline;
	int	interpret_escapes;
	char	*temp_str;

	i = 1;
	newline = 1;
	interpret_escapes = 0;
	while (argv[i] && argv[i][0] == '-')
	{
		if (ft_strncmp(argv[i], "-n", 2) == 0 && argv[i][2] == '\0')
		{
			newline = 0;
			i++;
		}
		else if (ft_strncmp(argv[i], "-e", 2) == 0 && argv[i][2] == '\0')
		{
			interpret_escapes = 1;
			i++;
		}
		else
			break;
	}
	while (argv[i])
	{
		if (interpret_escapes)
		{
			temp_str = ft_strdup(argv[i]);
			if (temp_str)
			{
				process_escape_sequences(temp_str);
				write(STDOUT_FILENO, temp_str, ft_strlen(temp_str));
				free(temp_str);
			}
		}
		else
			write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

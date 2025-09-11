








/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(char **args, t_data *data)
{
	char	cwd[PATH_MAX];

	(void)data;
	if (args[1] != NULL)
	{
		write(STDERR_FILENO, "pwd: too many arguments\n", 24);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		write(STDOUT_FILENO, cwd, ft_strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	perror("pwd");
	return (1);
}

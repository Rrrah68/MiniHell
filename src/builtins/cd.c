/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:17:46 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/15 18:34:58 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_oldcwd(char *oldcwd)
{
	if (getcwd(oldcwd, PATH_MAX) == NULL)
		return (perror("cd"), 1);
	return (0);
}

char	*resolve_target(char **argv, t_data *data)
{
	char	*home;

	if (argv[1] == NULL || argv[1][0] == '\0' || ft_strncmp(argv[1], "~",
			1) == 0)
	{
		home = get_env_value(data->env, "HOME");
		if (home == NULL)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (NULL);
		}
		return (home);
	}
	return (argv[1]);
}

int	builtin_cd(char **args, t_data *data)
{
	char	*target_dir;
	char	oldcwd[PATH_MAX];

	if (get_oldcwd(oldcwd) != 0)
		return (1);
	target_dir = resolve_target(args, data);
	if (target_dir == NULL)
		return (1);
	if (chdir(target_dir) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

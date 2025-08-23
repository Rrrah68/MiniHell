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

	if (argv[1] == NULL || argv[1][0] == '\0'
		|| ft_strncmp(argv[1], "~", 1) == 0)
	{
		home = get_env_value(data->env, "HOME");
		if (home == NULL)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
		return (home);
	}
	return (argv[1]);
}

static int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
		i++;
	return (i);
}

int	builtin_cd(char **args, t_data *data)
{
	char	*target_dir;
	char	oldcwd[PATH_MAX];
	char	newcwd[PATH_MAX];
	int		argc;

	if (!args || !data)
		return (1);
	/* 1) Refuser >1 argument (après expansion) comme bash */
	argc = count_args(args);
	if (argc > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		data->exit_status = 1;
		return (1);
	}
	/* 2) Sauvegarder OLDPWD avant chdir */
	if (get_oldcwd(oldcwd) != 0)
	{
		data->exit_status = 1;
		return (1);
	}
	/* 3) Résoudre la cible (HOME, ~, chaîne vide, etc.) */
	target_dir = resolve_target(args, data);
	if (target_dir == NULL)
	{
		data->exit_status = 1;
		return (1);
	}
	/* 4) Tenter le chdir ; message d'erreur au format bash-like */
	if (chdir(target_dir) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(target_dir, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		data->exit_status = 1;
		return (1);
	}
	/* 5) Mettre à jour OLDPWD et PWD après succès */
	add_env_var(&data->env, ft_strdup("OLDPWD"), ft_strdup(oldcwd));
	if (getcwd(newcwd, PATH_MAX) != NULL)
		add_env_var(&data->env, ft_strdup("PWD"), ft_strdup(newcwd));
	data->exit_status = 0;
	return (0);
}

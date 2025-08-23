/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:45:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/15 18:29:04 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args)
{
	int	status;

	/* aucun argument -> quitte 0 */
	if (!args[1])
		exit(0);

	/* si le 1er arg N'EST PAS numérique -> message exact + exit 2 */
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd(" numeric argument required\n", STDERR_FILENO);
		exit(2);
	}

	/* ici le 1er arg est numérique ; s'il y a un 2e arg -> erreur bash */
	if (args[2])
	{
		ft_putstr_fd(" too many arguments\n", STDERR_FILENO);
		return (1);
	}

	/* un seul arg numérique -> quitter avec (arg & 255) */
	status = ft_atoi(args[1]);
	exit(status & 255);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:45:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/10 17:17:52 by mobullad         ###   ########.fr       */
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

static int	check_overflow(char *str)
{
	int	len;
	int	i;

	len = ft_strlen(str);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		i++;
		len--;
	}
	if (len > 19)
		return (1);
	return (0);
}

int	builtin_exit(char **args)
{
	int	status;

	if (!args[1])
		exit(0);
	if (!is_numeric(args[1]) || check_overflow(args[1]))
	{
		ft_putstr_fd(" numeric argument required\n", STDERR_FILENO);
		exit(2);
	}
	if (args[2])
	{
		ft_putstr_fd(" too many arguments\n", STDERR_FILENO);
		exit(1);
	}
	status = ft_atoi(args[1]);
	exit(status & 255);
}

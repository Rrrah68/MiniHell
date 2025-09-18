/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:56:40 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 22:24:47 by mobullad         ###   ########.fr       */
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

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (!args[1])
	{
		g_signal_status = 0;
		return (-2);
	}
	if (!is_numeric(args[1]) || check_overflow(args[1]))
	{
		ft_putstr_fd("minishell: exit:numeric argument required\n",
			STDERR_FILENO);
		g_signal_status = 2;
		return (-2);
	}
	if (args[2])
	{
		ft_putstr_fd(" too many arguments\n", STDERR_FILENO);
		g_signal_status = 1;
		return (-2);
	}
	status = ft_atoi(args[1]);
	g_signal_status = status & 255;
	return (-2);
}

//exit dans ton exit encule
//return (unsigned char)status ;

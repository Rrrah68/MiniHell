/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/15 00:00:00 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_newline_option(char **argv)
{
	int	i;

	i = 1;
	while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n'
		&& argv[i][2] == '\0')
		i++;
	return (i > 1);
}

static int	skip_options(char **argv)
{
	int	i;

	i = 1;
	while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n'
		&& argv[i][2] == '\0')
		i++;
	return (i);
}

int	builtin_echo(char **argv)
{
	int	i;
	int	newline;

	newline = !has_newline_option(argv);
	i = skip_options(argv);
	while (argv[i])
	{
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

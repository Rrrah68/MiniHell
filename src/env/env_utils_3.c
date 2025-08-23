/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:31:23 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/15 18:31:24 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_env(char **env)
{
	size_t	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

size_t	find_env(char **env, const char *name)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(name);
	i = 0;
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], name, len) && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	*get_env(char **envp, const char *name)
{
	size_t	len;
	size_t	i;

	len = 0;
	while (name[len])
		len++;
	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

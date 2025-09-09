/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:31:25 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/15 18:35:17 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_entry(const char *name, const char *value)
{
	size_t	name_len;
	size_t	value_len;
	char	*entry;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	entry = malloc(name_len + value_len + 2);
	if (entry == NULL)
		return (NULL);
	ft_memcpy(entry, name, name_len);
	entry[name_len] = '=';
	ft_memcpy(entry + name_len + 1, value, value_len + 1);
	return (entry);
}

void	replace_env(char **env, size_t idx, char *entry)
{
	free(env[idx]);
	env[idx] = entry;
}

int	append_env(char **envp, char *entry)
{
	size_t	i;
	size_t	count;
	char	**new_env;

	count = count_env(*envp);
	new_env = malloc(sizeof(char *) * (count + 2));
	if (new_env == NULL)
		return (-1);
	i = 0;
	while ((*envp)[i])
	{
		new_env[i] = (*envp)[i];
		i++;
	}
	new_env[i++] = entry;
	new_env[i] = NULL;
	free(*envp);
	*envp = new_env;
	return (0);
}

void	set_env(char ***envp, const char *name, const char *value)
{
	char	*entry;
	size_t	idx;

	entry = build_entry(name, value);
	if (entry == NULL)
		return ;
	idx = find_env(*envp, name);
	if (idx != (size_t)-1)
		replace_env(*envp, idx, entry);
	else
		append_env(envp, entry);
}

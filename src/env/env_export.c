/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:55:34 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/16 15:55:35 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	compare_keys(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

static void	sort_env_array(t_env **env_array, int count)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (compare_keys(env_array[j]->key, env_array[j + 1]->key) > 0)
			{
				temp = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static t_env	**create_env_array(t_env *env_list, int count)
{
	t_env	**env_array;
	t_env	*current;
	int		i;

	env_array = malloc(sizeof(t_env *) * count);
	if (!env_array)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		env_array[i++] = current;
		current = current->next;
	}
	return (env_array);
}

static void	print_env_array(t_env **env_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (env_array[i]->value)
			ft_printf("declare -x %s=\"%s\"\n",
				env_array[i]->key, env_array[i]->value);
		else
			ft_printf("declare -x %s\n", env_array[i]->key);
		i++;
	}
}

void	print_exported_vars(t_env *env_list)
{
	t_env	**env_array;
	int		count;

	count = count_env_vars(env_list);
	if (count == 0)
		return ;
	env_array = create_env_array(env_list, count);
	if (!env_array)
		return ;
	sort_env_array(env_array, count);
	print_env_array(env_array, count);
	free(env_array);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_basic.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:55:41 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/16 15:55:42 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env_vars(t_env *env)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	*create_env_string(t_env *current)
{
	char	*temp;
	char	*result;

	if (current->value)
	{
		temp = ft_strjoin(current->key, "=");
		if (temp)
		{
			result = ft_strjoin(temp, current->value);
			free(temp);
			return (result);
		}
	}
	return (ft_strdup(current->key));
}

char	**env_to_array(t_env *env)
{
	t_env	*current;
	char	**array;
	int		count;
	int		i;

	count = count_env_vars(env);
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	array[count] = NULL;
	current = env;
	i = 0;
	while (current && i < count)
	{
		array[i] = create_env_string(current);
		current = current->next;
		i++;
	}
	return (array);
}

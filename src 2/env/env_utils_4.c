/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:50:00 by mobullad          #+#    #+#             */
/*   Updated: 2025/08/22 17:30:09 by mobullad         ###   ########.fr       */
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

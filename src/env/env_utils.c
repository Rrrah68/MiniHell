/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:55:37 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 21:18:53 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_environment(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		our_free(current->key);
		our_free(current->value);
		our_free(current);
		current = next;
	}
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	if (!env || !key)
		return (NULL);
	current = env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	add_env_var(t_env **env, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_node;

	if (!env || !key)
		return ;
	current = *env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
		{
			our_free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return ;
		}
		current = current->next;
	}
	new_node = create_env_node(key, value);
	if (!new_node)
		return ;
	new_node->next = *env;
	*env = new_node;
}

void	remove_env_var(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !*env || !key)
		return ;
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			our_free(current->key);
			our_free(current->value);
			our_free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

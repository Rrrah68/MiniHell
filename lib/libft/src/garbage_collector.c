/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:10:03 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 18:34:06 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	mem_manager_add_front(t_mem_mng **head_, t_mem_mng *node)
{
	node->next = *head_;
	*head_ = node;
}

t_mem_mng	*mem_manager_new_node(size_t bytes, int *error)
{
	t_mem_mng	*node;

	*error = 0;
	node = malloc(sizeof(t_mem_mng));
	if (!node)
	{
		*error = -1;
		return (NULL);
	}
	node->addr = malloc(bytes);
	if (!node->addr)
	{
		*error = -1;
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

void	mem_manager_del_node(t_mem_mng **root, void *address)
{
	t_mem_mng	*prev;
	t_mem_mng	*node;

	if (!address)
		return ;
	node = *root;
	prev = NULL;
	while (node && node->addr != address)
	{
		prev = node;
		node = node->next;
	}
	if (prev && node != NULL)
		prev->next = node->next;
	if (node == *root)
		*root = (*root)->next;
	if (node != NULL)
		free(node->addr);
	else
	{
		free(address);
	}
	free(node);
}

void	mem_manager_del_list(t_mem_mng **root)
{
	t_mem_mng	*node;
	t_mem_mng	*tmp;

	if (!root)
		return ;
	node = *root;
	while (node)
	{
		tmp = node->next;
		free(node->addr);
		free(node);
		node = tmp;
	}
	*root = NULL;
}

void	*safe_as_fuck_malloc(size_t bytes, void *address, int action)
{
	static t_mem_mng	*all_mallocs = NULL;
	t_mem_mng			*new_node;

	if (action >= SAFE_MALLOC_ALLOC)
	{
		new_node = mem_manager_new_node(bytes, &action);
		if (action == -1)
		{
			mem_manager_del_list(&all_mallocs);
			write(2, "Critical memory failure during allocation", 42);
			exit(1);
		}
		mem_manager_add_front(&all_mallocs, new_node);
	}
	else if (action == SAFE_MALLOC_FREE)
	{
		mem_manager_del_node(&all_mallocs, address);
		return (NULL);
	}
	else
	{
		mem_manager_del_list(&all_mallocs);
		return (NULL);
	}
	return (new_node->addr);
}

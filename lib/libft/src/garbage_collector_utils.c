/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:29:07 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 18:29:32 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*our_malloc(size_t bytes)
{
	void	*ret;

	ret = safe_as_fuck_malloc(bytes, NULL, SAFE_MALLOC_ALLOC);
	return (ret);
}

void	our_free(void *address)
{
	if (address == NULL)
		return ;
	safe_as_fuck_malloc(0, address, SAFE_MALLOC_FREE);
}

char	*unionize_str(char *theirs)
{
	char	*ours;

	ours = ft_strdup(theirs);
	free(theirs);
	return (ours);
}

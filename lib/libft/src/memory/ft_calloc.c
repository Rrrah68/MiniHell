/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:56:04 by mobullad          #+#    #+#             */
/*   Updated: 2024/11/15 15:18:37 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*k;

	if (nmemb != 0 && (nmemb * size) / nmemb != size)
		return (NULL);
	k = (malloc)(nmemb * size);
	if (!k)
		return (NULL);
	ft_bzero(k, size * nmemb);
	return (k);
}

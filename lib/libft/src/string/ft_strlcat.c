/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 18:44:37 by mobullad          #+#    #+#             */
/*   Updated: 2024/11/15 16:00:11 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	k;
	size_t	j;

	if (!src || size == 0)
		return (ft_strlen(src));
	i = 0;
	k = ft_strlen(dest);
	j = ft_strlen(src);
	if (size <= k)
		return (size + j);
	while (i < size - k - 1 && src[i])
	{
		dest[k + i] = src[i];
		i++;
	}
	dest[k + i] = '\0';
	return (j + k);
}

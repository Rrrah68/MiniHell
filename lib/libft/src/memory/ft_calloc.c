/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:56:04 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 16:52:11 by radahman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*k;

	if (nmemb != 0 && (nmemb * size) / nmemb != size)
		return (NULL);
	k = (our_malloc)(nmemb * size);
	if (!k)
		return (NULL);
	ft_bzero(k, size * nmemb);
	return (k);
}

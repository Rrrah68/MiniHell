/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:57:25 by mobullad          #+#    #+#             */
/*   Updated: 2025/04/20 07:49:56 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*tab;
	int		i;
	int		k;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	k = 0;
	tab = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!tab)
		return (NULL);
	while (s1[i] != '\0')
		tab[k++] = s1[i++];
	i = 0;
	while (s2[i] != '\0')
		tab[k++] = s2[i++];
	tab[k] = '\0';
	return (tab);
}

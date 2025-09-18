/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radahman <radahman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:40:15 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 16:34:39 by radahman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(const char *s, char c)
{
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (s && s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
			nb++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (nb);
}

static int	ft_split_2(const char *s, int len, int *k, char **tab)
{
	tab[*k] = ft_substr(s, 0, len);
	if (!tab[*k])
	{
		while (*k > 0)
		{
			(*k)--;
			our_free(tab[*k]);
		}
		our_free(tab);
		return (0);
	}
	(*k)++;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		i;
	int		j;
	int		k;

	if (!s)
		return (NULL);
	k = 0;
	i = 0;
	j = 0;
	tab = our_malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!tab)
		return (NULL);
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		j = i;
		while (s[i] && s[i] != c)
			i++;
		if (i != j && !ft_split_2(s + j, i - j, &k, tab))
			return (NULL);
	}
	return (tab[k] = NULL, tab);
}

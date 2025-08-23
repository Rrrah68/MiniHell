/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 14:59:08 by mobullad          #+#    #+#             */
/*   Updated: 2024/11/04 17:46:33 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		i;
	t_list	*str;

	i = 0;
	str = lst;
	if (str == NULL)
		return (0);
	while (str)
	{
		str = str->next;
		i++;
	}
	return (i);
}

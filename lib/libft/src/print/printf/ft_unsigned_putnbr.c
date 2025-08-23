/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsigned_putnbr.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:15:48 by mobullad          #+#    #+#             */
/*   Updated: 2025/05/19 20:13:19 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_unsigned_putnbr(unsigned int n)
{
	unsigned int	nb;

	nb = 0;
	if (n <= 9)
		return (ft_putchar((n + 48)));
	else
	{
		nb += ft_unsigned_putnbr(n / 10);
		nb += ft_unsigned_putnbr(n % 10);
	}
	return (nb);
}

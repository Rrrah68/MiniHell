/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:06:08 by mobullad          #+#    #+#             */
/*   Updated: 2025/05/19 20:10:57 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_ptr(unsigned long nb)
{
	int	len;

	len = 0;
	if (nb)
	{
		len += ft_putstr("0x");
		len += ft_print_hex(nb, 0);
	}
	else
		len += ft_putstr("(nil)");
	return (len);
}

int	ft_print_hex(unsigned long nb, int i)
{
	int	res;

	res = 0;
	if (nb == 0)
	{
		res += ft_putchar('0');
		return (res);
	}
	if (nb >= 16)
		res += ft_print_hex(nb / 16, i);
	if (nb % 16 < 10)
		res += ft_putchar('0' + (nb % 16));
	else
	{
		if (i)
			res += ft_putchar(nb % 16 - 10 + 'A');
		else
			res += ft_putchar(nb % 16 - 10 + 'a');
	}
	return (res);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:40:06 by mobullad          #+#    #+#             */
/*   Updated: 2025/05/19 20:10:57 by mobullad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_format_check(char c, va_list args)
{
	int	print_char;

	print_char = 0;
	if (c == 'c')
		print_char = ft_putchar(va_arg(args, int));
	else if (c == 's')
		print_char = ft_putstr(va_arg(args, char *));
	else if (c == 'p')
		print_char += ft_print_ptr(va_arg(args, unsigned long));
	else if (c == 'd' || c == 'i')
		print_char = ft_putnbr(va_arg(args, int));
	else if (c == 'u')
		print_char = ft_unsigned_putnbr(va_arg(args, unsigned int));
	else if (c == 'x')
		print_char = ft_print_hex(va_arg(args, unsigned int), 0);
	else if (c == 'X')
		print_char = ft_print_hex(va_arg(args, unsigned int), 1);
	else if (c == '%')
		print_char = ft_putchar('%');
	return (print_char);
}

int	ft_printf(const char *string, ...)
{
	va_list	args;
	int		i;
	int		j;

	i = 0;
	j = 0;
	va_start(args, string);
	while (string[i])
	{
		if (string[i] == '%' && string[i + 1])
		{
			i++;
			j += ft_format_check(string[i], args);
		}
		else
		{
			ft_putchar(string[i]);
			j++;
		}
		i++;
	}
	va_end(args);
	return (j);
}

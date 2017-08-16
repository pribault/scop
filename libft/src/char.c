/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/10 11:39:46 by pribault          #+#    #+#             */
/*   Updated: 2017/07/19 13:41:50 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		print_char(char c)
{
	write(1, &c, 1);
	return (1);
}

int		print_wchar(wchar_t c)
{
	if (c <= 0x7F)
	{
		ft_putchar(c);
		return (1);
	}
	if (c <= 0x7FF)
	{
		ft_putchar((c >> 6) + 0xC0);
		ft_putchar((c & 0x3F) + 0x80);
		return (2);
	}
	if (c <= 0xFFFF)
	{
		ft_putchar((c >> 12) + 0xE0);
		ft_putchar(((c >> 6) & 0x3F) + 0x80);
		ft_putchar((c & 0x3F) + 0x80);
		return (3);
	}
	ft_putchar((c >> 18) + 0xF0);
	ft_putchar(((c >> 12) & 0x3F) + 0x80);
	ft_putchar(((c >> 6) & 0x3F) + 0x80);
	ft_putchar((c & 0x3F) + 0x80);
	return (4);
}

void	if_forest_char(va_list va, t_param param, int *len, int state)
{
	wchar_t	c;

	c = (state < 0) ? va_arg(va, wchar_t) : state;
	c = (state == -2 && param.modif < 0x100) ? (char)c : c;
	*len += print_params(&param, 1, '0', 2);
	*len += print_wchar(c);
	*len += print_params(&param, 1, ' ', -1);
}

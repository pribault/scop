/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/10 15:08:51 by pribault          #+#    #+#             */
/*   Updated: 2018/06/27 13:25:00 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	error(int error, void *param, char state)
{
	ft_putstr("\033[3m\033[48;5;88m[error]\033[0m\033[3m\033[38;5;196m ");
	if (error == 0)
		ft_putstr("usage: ./scop <file>");
	else if (error == 1)
		ft_putstr("cannot allocate memory");
	else if (error == 2)
		ft_printf("cannot open file %s", param);
	else if (error == 3)
		ft_printf("cannot init sdl2: %s", SDL_GetError());
	else if (error == 4)
		ft_printf("no display available: %s", SDL_GetError());
	else if (error == 5)
		ft_printf("cannot get infos about display: %s", SDL_GetError());
	else if (error == 6)
		ft_printf("cannot create sdl window: %s", SDL_GetError());
	else if (error == 7)
		ft_printf("cannot create opengl context %s", SDL_GetError());
	else if (error == 8)
		ft_printf("cannot set attribute: %s", SDL_GetError());
	else if (error == 9)
		ft_printf("invalid file %s", param);
	else if (error == 100)
		ft_printf("instruction %s not recognized", param);
	else if (error == 101)
		ft_putstr("wrong id in object, aborting");
	else if (error == 102)
		ft_printf("cannot find material %s", param);
	else
		ft_putstr("unknown error");
	ft_putstr("\033[0m\n");
	if (state)
		exit(1);
}

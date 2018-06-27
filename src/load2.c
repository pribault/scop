/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 18:16:11 by pribault          #+#    #+#             */
/*   Updated: 2018/06/27 13:22:31 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

size_t	ft_listlen(t_list *list)
{
	size_t	i;

	i = 0;
	while (list)
	{
		i++;
		list = list->next;
	}
	return (i);
}

t_list	*get_listn(t_list *list, size_t n)
{
	size_t	i;

	i = ft_listlen(list);
	while (list)
	{
		if (i == n)
			return (list);
		i--;
		list = list->next;
	}
	error(101, NULL, 1);
	return (NULL);
}

void	get_numbers(char *params, size_t *n)
{
	size_t	i;

	i = 0;
	n[0] = (ft_strlen(params)) ? ft_atoi(params) : 0;
	while (ft_isdigit(params[i]))
		i++;
	if (params[i] == '/')
		i++;
	n[1] = (ft_strlen(params + i)) ? ft_atoi(params + i) : 0;
	while (ft_isdigit(params[i]))
		i++;
	if (params[i] == '/')
		i++;
	n[2] = (ft_strlen(params + i)) ? ft_atoi(params + i) : 0;
}

t_mat	*get_mat(t_list *list, char *name)
{
	t_mat	*current;

	while (list)
	{
		current = list->content;
		if (!ft_strcmp(name, current->name))
			return (current);
		list = list->next;
	}
	error(102, name, 0);
	return (NULL);
}

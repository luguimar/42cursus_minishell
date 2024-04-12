/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:18:00 by luguimar          #+#    #+#             */
/*   Updated: 2023/11/18 03:40:24 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list **head, t_list *lst, void (*del)(void *))
{
	if (head && lst && del)
	{
		del(lst->content);
		if (*head == lst)
			*head = lst->next;
		if (lst->next)
		{
			if (lst->prev)
				lst->next->prev = lst->prev;
			else
				lst->next->prev = NULL;
		}
		if (lst->prev)
		{
			if (lst->next)
				lst->prev->next = lst->next;
			else
				lst->prev->next = NULL;
		}
		free(lst);
	}
}

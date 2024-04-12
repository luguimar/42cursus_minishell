/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 18:05:47 by luguimar          #+#    #+#             */
/*   Updated: 2023/12/07 18:57:44 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *novel)
{
	if (!*lst)
	{
		*lst = novel;
		(*lst)->prev = NULL;
		(*lst)->next = NULL;
	}
	else
	{
		novel->next = NULL;
		if (*lst == NULL)
		{
			novel->prev = NULL;
			*lst = novel;
			return ;
		}
		novel->prev = ft_lstlast(*lst);
		ft_lstlast(*lst)->next = novel;
	}
}

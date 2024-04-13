/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_size_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:51:52 by luguimar          #+#    #+#             */
/*   Updated: 2023/12/19 16:52:53 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	bigger(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	smaller(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

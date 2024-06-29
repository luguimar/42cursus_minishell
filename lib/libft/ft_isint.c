/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isint.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 23:56:54 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/28 08:59:22 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isint(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	if (ft_strlen(str) > 11)
		return (0);
	if (ft_strlen(str) == 11 && str[0] != '-')
		return (0);
	if (ft_strlen(str) == 11 && str[0] == '-')
	{
		if (ft_atol(str) < -2147483648)
			return (0);
	}
	return (1);
}

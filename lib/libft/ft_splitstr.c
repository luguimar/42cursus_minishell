/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitstr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 02:26:06 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/26 02:26:16 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_splitstr(char *str, char *split)
{
	char	**return_array;
	char	*array;
	char	*tmp;
	int		i;
	int		j;

	tmp = str;
	array = ft_strnstr(str, split, ft_strlen(str));
	if (array == NULL)
		return (NULL);
	i = 0;
	while (tmp != array)
	{
		tmp++;
		i++;
	}
	j = 0;
	while (tmp[j] == split[j] && split[j] != '\0')
		j++;
	i += j;
	return_array = malloc(sizeof(char *) * 3);
	return_array[0] = ft_substr(str, 0, i);
	return_array[1] = ft_substr(str, i + j, ft_strlen(str));
	return_array[2] = NULL;
	return (return_array);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dup_array_of_strings.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 21:08:23 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/25 21:09:44 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_dup_array_of_strings(char **array)
{
	char	**new_array;
	int		i;

	i = 0;
	while (array[i])
		i++;
	new_array = (char **)malloc(sizeof(char *) * (i + 1));
	if (!new_array)
		return (NULL);
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			free_array_of_strings(new_array);
			return (NULL);
		}
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

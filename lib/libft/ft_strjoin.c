/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 16:58:09 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/24 19:17:48 by jduraes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strlenplus(char *s, char c)
{
	int	i;

	i = 0;
	if (c == '\n')
	{
		while (s && s[i] != '\0' && s[i] != '\n')
			i++;
		if (s[i] == c)
			return (i);
		return (-1);
	}
	while (s && s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strjoinfreeall(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*str;

		str = malloc((ft_strlenplus(s1, 0) + ft_strlenplus(s2, 0) + 1)
		* sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (s1 && s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j] != '\0')
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	free(s1);
	free(s2);
	return (str);
}

char	*ft_strjoinfree2(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*str;

	str = malloc((ft_strlenplus(s1, 0) + ft_strlenplus(s2, 0) + 1)
		* sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (s1 && s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j] != '\0')
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	free(s2);
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*str;

	str = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}

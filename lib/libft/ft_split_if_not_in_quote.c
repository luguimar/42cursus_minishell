/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_if_not_in_quote.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 21:39:52 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/27 04:36:31 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	wordcounter(char *s, char c)
{
	int	wordcount;
	int	i;

	if (!s)
		return (0);
	i = 0;
	wordcount = 0;
	while (s[i] != '\0')
	{
		if (!is_c_not_in_quotes(s, i, c) && (is_c_not_in_quotes(s, i + 1, c) \
		|| s[i + 1] == '\0'))
			wordcount++;
		i++;
	}
	return (wordcount);
}

static void	wordfiller(char *j, char *str, char c)
{
	int	i;

	i = 0;
	while (j[i] != '\0' && !is_c_not_in_quotes(j, i, c))
	{
		str[i] = j[i];
		i++;
	}
	str[i] = '\0';
}

static void	strfiller(char *s, char **str, char c)
{
	int	i;
	int	j;
	int	k;
	int	nextc;

	i = -1;
	k = 0;
	nextc = 0;
	while (s[++i] != '\0')
	{
		if (!is_c_not_in_quotes(s, i, c) && \
		(is_c_not_in_quotes(s, i + 1, c) || s[i + 1] == '\0'))
		{
			j = i + 1;
			while (--j >= nextc)
			{
				if (((j == 0 && s[j] != c) || is_c_not_in_quotes(s, j - 1, c)))
				{
					nextc = j + 1;
					wordfiller(s + j, str[k], c);
					k++;
				}
			}
		}
	}
}

static void	strmalloc(int *j, char ***str, int *k)
{
	(*str)[*k] = malloc((*j + 1) * sizeof(char));
	if (!(*str)[*k])
	{
		while (*k > 0)
		{
			*k = (*k) - 1;
			free((void *)(*str)[*k]);
			(*str)[*k] = NULL;
		}
		free(*str);
		*str = NULL;
	}
	*k = (*k) + 1;
	*j = 0;
}

char	**ft_split_if_not_in_quote(char *s, char c)
{
	int		i;
	int		j;
	int		k;
	char	**str;

	i = -1;
	j = 1;
	k = 0;
	str = malloc((wordcounter(s, c) + 1) * sizeof(char *));
	if (!str || !s)
		return (NULL);
	str[wordcounter(s, c)] = NULL;
	while (s[++i] != '\0')
	{
		if (!is_c_not_in_quotes(s, i, c) && \
		(is_c_not_in_quotes(s, i + 1, c) || s[i + 1] == '\0'))
		{
			strmalloc(&j, &str, &k);
			if (!str)
				return (NULL);
		}
		if (!is_c_not_in_quotes(s, i, c))
			j++;
	}
	return (strfiller((char *)s, str, c), str);
}

/*static int	wordcounter(char *s, char c)
{
	int	wordcount;
	int	i;

	if (!s)
		return (0);
	i = 0;
	wordcount = 0;
	while (s[i] != '\0')
	{
		if (s[i + 1] == '\0' || (s[i + 1] == c && !has_open_quote(s, i + 1)))
			wordcount++;
		i++;
	}
	return (wordcount);
}

static void	wordfiller(char *j, char *str, char c)
{
	int	i;

	i = 0;
	while ((j[i] != c || (j[i] == c && has_open_quote(j, i))) && j[i] != '\0')
	{
		str[i] = j[i];
		i++;
	}
	str[i] = '\0';
}

static void	strfiller(char *s, char **str, char c)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (s[i] != '\0')
	{
		if (s[i + 1] == '\0' || s[i + 1] == c)
		{
			j = i + 1;
			while (--j)
			{
				if ((s[j - 1] == c && !has_open_quote(s, j - 1)) \
					|| (j == 0 && s[j] != c))
				{
					wordfiller(s + j, str[k], c);
					k++;
				}
			}
		}
		i++;
	}
}

static void	strmalloc(int *j, char ***str, int *k)
{
	(*str)[*k] = malloc((*j + 1) * sizeof(char));
	if (!(*str)[*k])
	{
		while (*k > 0)
		{
			*k = (*k) - 1;
			free((void *)(*str)[*k]);
			(*str)[*k] = NULL;
		}
		free(*str);
		*str = NULL;
	}
	*k = (*k) + 1;
	*j = 0;
}

char	**ft_split_if_not_in_quote(char *s, char c)
{
	int		i;
	int		j;
	int		k;
	char	**str;

	i = -1;
	j = 1;
	k = 0;
	str = malloc((wordcounter(s, c) + 1) * sizeof(char *));
	if (!str || !s)
		return (NULL);
	str[wordcounter(s, c)] = NULL;
	while (s[++i] != '\0')
	{
		if (s[i + 1] == '\0' || (s[i + 1] == c && !has_open_quote(s, i + 1)))
		{
			strmalloc(&j, &str, &k);
			if (!str)
				return (NULL);
		}
		if (s[i] != c)
			j++;
	}
	strfiller((char *)s, str, c);
	return (str);
}*/
/*
int	main(void)
{
	char	**str;
	char	input[1000];
	int		i;

	i = 0;
	scanf("%s", input);
	str = ft_split_if_not_in_quote(input, ' ');
	while (str[i])
	{
		printf("%s\n", str[i]);
		i++;
	}
	return (0);
}*/

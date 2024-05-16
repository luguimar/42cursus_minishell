/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitquote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 20:46:24 by luguimar          #+#    #+#             */
/*   Updated: 2024/05/03 01:25:16 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_quote_count(char *str)
{
	int		quote_count;
	char	quote;
	int		i;

	quote_count = 0;
	quote = '\0';
	i = 0;
	while (str[i] != '\0')
	{
		if (quote == '\0' && (str[i] == '\'' || str[i] == '\"'))
		{
			quote = str[i];
			quote_count++;
		}
		else if (str[i] == quote && quote != '\0')
		{
			quote_count++;
			quote = '\0';
		}
		i++;
	}
	return (quote_count);
}

static void	ft_remove_quotes_aux(char *str, char *new)
{
	char	quote;
	int		i;
	int		j;

	quote = '\0';
	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (quote == '\0' && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		else if (str[i] == quote)
			quote = '\0';
		else
		{
			new[j] = str[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
}

char	*ft_remove_quotes(char *str)
{
	int		quote_count;
	char	*new;

	quote_count = ft_quote_count(str);
	new = malloc((ft_strlen(str) + 1 - quote_count) * sizeof(char));
	if (!new)
		return (NULL);
	ft_remove_quotes_aux(str, new);
	free(str);
	return (new);
}

char	**ft_splitquote(char *s, char c)
{
	char	**str;
	int		i;

	str = ft_split_if_not_in_quote(s, c);
	if (!str)
		return (NULL);
	i = 0;
	while (str[i] != NULL)
	{
		str[i] = ft_remove_quotes(str[i]);
		if (!str[i])
		{
			while (i > 0)
			{
				i--;
				free(str[i]);
				str[i] = NULL;
			}
			free(str);
			return (NULL);
		}
		i++;
	}
	return (str);
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
		if (is_end_wordquote(s, i, c))
			wordcount++;
		i++;
	}
	return (wordcount);
}

static void	wordfiller(char *j, char *str, char c, int k)
{
	int	i;

	i = 0;
	if (k != 0)
	{
		while (inquote(j - 1, i + 1) || (!quote(j - 1, i + 1) && j[i] != c \
		&& j[i] != '\0'))
		{
			str[i] = j[i];
			i++;
		}
	}
	else
	{
		while (inquote(j, i) || (!quote(j, i) && j[i] != c && j[i] != '\0'))
		{
			str[i] = j[i];
			i++;
		}
	}
	str[i] = '\0';
}

static void	strfiller(char *s, char **str, char c)
{
	int	i;
	int	j;
	int	k;
	int	nextc;

	i = 0;
	k = 0;
	nextc = 0;
	while (s[i] != '\0')
	{
		if (is_end_wordquote(s, i, c))
		{
			j = i + 1;
			while (j-- >= nextc)
			{
				if (is_beginning_wordquote(s, j, c))
				{
					nextc = j + 1;
					wordfiller(s + j, str[k], c, j);
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

char	**ft_splitquote(char *s, char c)
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
		if (is_end_wordquote(s, i, c))
		{
			strmalloc(&j, &str, &k);
			if (!str)
				return (NULL);
		}
		if (inquote(s, i) || (!quote(s, i) && s[i] != c))
			j++;
	}
	strfiller((char *)s, str, c);
	return (str);
}*/
/*
#include <stdio.h>

int	main(int argc, char *argv[])
{
	int	i;

	i = 0;
	if (argc == 3)
	{
		char **result = ft_split(argv[1], argv[2][0]);
		if (result)
		{
			while (result[i] != NULL)
			{
				printf("%s$\n", result[i]);
				i++;
			}
			i = 0;
			while (result[i] != NULL)
			{
				free(result[i]);
				i++;
			}
			free(result);
		}
		else
			printf("a alocaçao de memoria falhou");
	}
	else
		printf("numero invalido de argumentos");
	return (0);
}*/

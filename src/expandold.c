/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 18:28:57 by jduraes-          #+#    #+#             */
/*   Updated: 2024/04/26 08:58:23 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	expand_aux_aux(char ***keyandmore, t_shell *shell, int i)
{
	char	*value;
	int		keylen;
	char	*prev;
	char	*extra;
	char	*key;

	keylen = 0;
	value = NULL;
	if ((**keyandmore)[i] == '?')
	{
		keylen = 1;
		value = ft_itoa(shell->exit_status);
		prev = ft_substr(**keyandmore, 0, i - 1);
		extra = ft_substr(**keyandmore, i + keylen, ft_strlen(**keyandmore) - i \
			- keylen);
		free(**keyandmore);
		**keyandmore = ft_strjoinfreeall(prev, value);
		**keyandmore = ft_strjoinfreeall(**keyandmore, extra);
		return (1);
	}
	else
	{
		while ((**keyandmore)[i + keylen] != '\0' && !ft_is_special_char \
		((**keyandmore)[i + keylen]))
			keylen++;
		if (keylen == 0)
			return (0);
		key = ft_substr(**keyandmore, i, keylen);
		if (get_env_value(shell->env, key) != NULL)
			value = ft_strdup(get_env_value(shell->env, key));
		prev = ft_substr(**keyandmore, 0, i - 1);
		extra = ft_substr(**keyandmore, i + keylen, ft_strlen(**keyandmore) - i \
			- keylen);
		free(**keyandmore);
		if (value == NULL)
			**keyandmore = ft_strjoinfreeall(prev, extra);
		else
		{
			**keyandmore = ft_strjoinfreeall(prev, value);
			**keyandmore = ft_strjoinfreeall(**keyandmore, extra);
		}
		free(key);
		return (1);
	}
}

static int	expand_aux(char **input, t_shell *shell, int j)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while ((*input)[i] != '\0' && count <= j)
	{
		if ((*input)[i] == '$' && inquote(*input, i) != '\'')
			count++;
		i++;
	}
	if ((*input)[i - 1] == '$' && inquote(*input, i - 1) != '\'')
		return (expand_aux_aux(&input, shell, i));
	else
		return (0);
}

void	expand(char **input, t_shell *shell)
{
	int		j;
	int		dollar_count;

	j = 0;
	dollar_count = 0;
	while ((*input)[j] != '\0')
	{
		if ((*input)[j] == '$' && inquote(*input, j) != '\'')
			dollar_count++;
		j++;
	}
	if (dollar_count == 0)
		return ;
	j = 0;
	while (j < dollar_count)
	{
		if (expand_aux(input, shell, j))
		{
			j--;
			dollar_count--;
		}
		j++;
	}
}

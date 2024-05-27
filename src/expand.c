/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 19:58:13 by jduraes-          #+#    #+#             */
/*   Updated: 2024/05/27 17:26:12 by jduraes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*expand_aux(char *key, t_shell *shell)
{
	char	*value;

	if (get_env_value(shell->env, key) != NULL)
	{
		value = ft_strdup(get_env_value(shell->env, key));
		free(key);
		return (value);
	}
	else
	{
		free(key);
		return (NULL);
	}
}

static char	*expandqm(char **input, char *key, int i, int *s)
{
	char	*new;

	if (key)
	{
		new = ft_strjoinfreeall(ft_substr(*input, 0, i), key);
		new = ft_strjoinfreeall(new, \
			ft_substr(*input, i + *s + 1, ft_strlen(*input) - (i + *s)));
		free(*input);
		*s = 0;
		*input = ft_strdup(new);
		return (new);
	}
	else
	{
		*s = 0;
		return (ft_strdup(*input));
	}
}

void	expand(char **input, t_shell *shell, int i, int s)
{
	char	*tinput;

	tinput = ft_strdup(*input);
	while (tinput[++i] != '\0')
	{
		if (tinput[i] == '$' && inquote(tinput, i) != '\'')
		{
			while (tinput[i + 1 + s] != '\0'
				&& !ft_is_special_char(tinput[i + 1 + s]))
				s++;
			if (s)
			{
				free(*input);
				*input = expandqm(&tinput, \
					expand_aux(ft_substr(tinput, i + 1, s), shell), i, &s);
			}
			else if (s++ == 0 && tinput[i + 1] == '?' && tinput[i] != '\0')
			{
				free(*input);
				*input = expandqm(&tinput, \
					ft_itoa(shell->exit_status), i, &s);
			}
		}
	}
	free(tinput);
}

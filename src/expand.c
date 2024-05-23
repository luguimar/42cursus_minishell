/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 19:58:13 by jduraes-          #+#    #+#             */
/*   Updated: 2024/05/23 21:36:40 by jduraes-         ###   ########.fr       */
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
		return (NULL);
}

static char	*expandqm(char **input, char *key, int *i, int *s)
{
	char	*new;

	if (key)
	{
		new = ft_strjoinfreeall(ft_substr(*input, 0, *i), key);
		new = ft_strjoinfreeall(new, \
			ft_substr(*input, *i + *s + 1, ft_strlen(*input) - (*i + *s)));
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

void	expand(char **inputt, t_shell *shell, int i, int s)
{
	char	*input;

	input = ft_strdup(*inputt);
	while (input[++i] != '\0')
	{
		if (input[i] == '$' && inquote(input, i) != '\'')
		{
			while (input[i + 1 + s] != '\0' && \
				!ft_is_special_char(input[i + 1 + s]))
				s++;
			if (s)
			{
				free(*inputt);
				*inputt = expandqm(&input, \
					expand_aux(ft_substr(input, i + 1, s), shell), &i, &s);
			}
			else if (input[i + 1] == '?')
			{
				s++;
				free(*inputt);
				*inputt = expandqm(&input, \
					ft_itoa(shell->exit_status), &i, &s);
			}
		}
	}
	if (input)
		free(input);
}

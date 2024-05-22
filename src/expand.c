/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 19:58:13 by jduraes-          #+#    #+#             */
/*   Updated: 2024/05/22 20:37:06 by jduraes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*expand_aux(char *key, t_shell *shell)
{
	if (get_env_value(shell->env, key) != NULL)
		return (ft_strdup(get_env_value(shell->env, key)));
	else
		return (NULL);
}

static char	*expandqm(char **input, char *key, int i, int s)
{
	char	*new;

	new = ft_strjoinfreeall(ft_substr(*input, 0, i), key);
	new = ft_strjoinfreeall(new, \
		ft_substr(*input, i + s + 1, ft_strlen(*input) - (i + s)));
	free(*input);
	*input = ft_strdup(new);
	return (new);
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
				*inputt = expandqm(&input, \
					expand_aux(ft_substr(input, i + 1, s), shell), i, s);
			}
			else if (input[i + 1] == '?')
				*inputt = expandqm(&input, ft_itoa(shell->exit_status), i, s + 1);
			i = 0;
			s = 0;
		}
	}
}
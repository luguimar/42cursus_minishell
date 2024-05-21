/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 19:58:13 by jduraes-          #+#    #+#             */
/*   Updated: 2024/05/21 21:53:18 by jduraes-         ###   ########.fr       */
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

void	expand(char **inputt, t_shell *shell)
{
	int	i;
	int	s;
	char	*new;
	char	*input;

	i = 0;
	s = 0;
	input = ft_strdup(*inputt);
	while (input[i] != '\0')
	{
		if (input[i] == '$' && inquote(input, i) != '\'')
		{
			while (input[i + 1 + s] != '\0' && \
				!ft_is_special_char(input[i + 1 + s]))
				s++;
			if (s)
			{
				new = ft_strjoin(ft_substr(input, 0, i), \
					expand_aux(ft_substr(input, i + 1, s), shell));
				*inputt = ft_strjoin(new, \
					ft_substr(input, i + s + 1, ft_strlen(input) - (i + s)));
			}
		}
		i++;
	}
}
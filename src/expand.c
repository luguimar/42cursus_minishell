/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 19:58:13 by jduraes-          #+#    #+#             */
/*   Updated: 2024/06/11 21:04:06 by luguimar         ###   ########.fr       */
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

static void	expandqm(char **input, char *key, int *i, int *s)
{
	char	*new;

	if (key)
	{
		new = ft_strjoinfreeall(ft_substr(*input, 0, *i), key);
		new = ft_strjoinfreeall(new, \
			ft_substr(*input, *i + *s + 1, ft_strlen(*input) - (*i + *s)));
		free(*input);
		*s = 0;
		*input = new;
	}
	else
	{
		new = ft_strjoinfreeall(ft_substr(*input, 0, *i), \
			ft_substr(*input, *i + *s + 1, ft_strlen(*input) - (*i + *s)));
		free(*input);
		*input = new;
		*s = 0;
		(*i)--;
	}
}

/*void	expand(t_shell *shell, int i, int s)
{
	while (shell->input[++i] != '\0')
	{
		if (shell->input[i] == '$' && inquote(shell->input, i) != '\'')
		{
			while (shell->input[i + 1 + s] != '\0'
				&& !ft_is_special_char(shell->input[i + 1 + s]))
				s++;
			if (s)
			{
				expandqm(&shell->input, \
					expand_aux(ft_substr(shell->input, i + 1, s), \
						shell), i, &s);
			}
			else if (shell->input[i + 1] == '?' && shell->input[i] != '\0')
			{
				s++;
				expandqm(&shell->input, \
					ft_itoa(shell->exit_status), i, &s);
			}
		}
	}
}*/

void	expand(char **input, t_shell *shell, int i, int s)
{
	while ((*input)[++i] != '\0')
	{
		if ((*input)[i] == '$' && inquote(*input, i) != '\'')
		{
			while ((*input)[i + 1 + s] != '\0' && !ft_is_special_char
			((*input)[i + 1 + s]))
			{
				s++;
				if (ft_isdigit((*input)[i + 1]))
					break ;
			}
			if (s)
			{
				expandqm(input, \
					expand_aux(ft_substr(*input, i + 1, s), \
						shell), &i, &s);
			}
			else if ((*input)[i + 1] == '?' && (*input)[i] != '\0')
			{
				if ((*input)[i + 1] == '?')
					s++;
				expandqm(input, ft_itoa(shell->exit_status), &i, &s);
			}
		}
	}
}

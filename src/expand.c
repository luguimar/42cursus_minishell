/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 18:28:57 by jduraes-          #+#    #+#             */
/*   Updated: 2024/04/25 23:16:19 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	freenull(char **str)
{
	free(*str);
	*str = NULL;
}

static char	*expand_aux(char *key, t_list *env)
{
	char	*value;
	char	*newkey;
	t_list	*tmp;
	t_env	*env_var;

	tmp = env;
	if (key[0] == '\'')
		newkey = ft_substr(key, 2, ft_strlen(key) - 1);
	else
		newkey = ft_substr(key, 1, ft_strlen(key) - 1);
	free(key);
	while (tmp->next)
	{
		env_var = (t_env *)tmp->content;
		if (ft_strncmp(env_var->key, newkey, ft_strlen(newkey)) == 0)
		{
			value = ft_strdup(env_var->value);
			free(newkey);
			return (value);
		}
		tmp = tmp->next;
	}
	free(newkey);
	return (NULL);
}

void	expand(t_shell *shell)
{
	char	*newarg;
	char	**args;
	int		i;

	i = 0;
	if (ft_strlen(shell->input) == 0)
		return ;
	args = ft_split(shell->input, ' ');
	newarg = NULL;
	while (args[i] != NULL)
	{
		if (args[i][0] == '$' || (args[i][0] == '\'' && args[i][1] == '$' \
		&& args[i][ft_strlen(args[i] - 1)] == '\''))
		{
			newarg = expand_aux(args[i], shell->env);
			if (newarg == NULL)
				newarg = ft_strdup("");
			args[i] = ft_strdup(newarg);
			freenull(&newarg);
		}
		i++;
	}
	i = 0;
	newarg = ft_strdup(args[i++]);
	free(args[i - 1]);
	while (args[i] != NULL)
	{
		newarg = ft_strjoinfree(newarg, " ");
		newarg = ft_strjoinfreeall(newarg, args[i++]);
	}
	free(shell->input);
	shell->input = newarg;
	free(args);
}

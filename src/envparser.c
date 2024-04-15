/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envparser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 23:50:43 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/16 00:39:03 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*envnew(char *key, char *value, char *env_i)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->full = ft_strdup(env_i);
	if (!new->full)
		return (NULL);
	return (new);
}

int	env_to_list(t_shell *shell, char **envp)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

	i = 0;
	shell->env = NULL;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		key = ft_substr(envp[i], 0, j);
		value = ft_strdup(&envp[i][j + 1]);
		if (!key || !value)
			return (0);
		ft_lstadd_back(&shell->env, \
			ft_lstnew((void *)envnew(key, value, envp[i])));
		i++;
	}
	return (1);
}

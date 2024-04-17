/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envparser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 23:50:43 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/17 09:37:22 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	change_value(t_list *env, char *key, char *value)
{
	while (env)
	{
		if (!ft_strcmp(((t_env *)env->content)->key, key))
		{
			free(((t_env *)env->content)->value);
			((t_env *)env->content)->value = value;
			free(((t_env *)env->content)->full);
			((t_env *)env->content)->full = ft_strjoin(key, \
				ft_strjoin("=", value));
			return ;
		}
		env = env->next;
	}
}

void	add_env(t_shell *shell, char *key, char *value)
{
	ft_lstadd_back(&shell->env, ft_lstnew((void *)envnew(key, value, \
		ft_strjoin(key, ft_strjoin("=", value)))));
}

char	**env_to_array(t_list *env)
{
	int		i;
	char	**envp;
	t_list	*tmp;

	tmp = env;
	i = 0;
	envp = (char **)malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	if (!envp)
		return (NULL);
	while (tmp)
	{
		envp[i] = ((t_env *)tmp->content)->full;
		tmp = tmp->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

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

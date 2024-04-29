/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envparser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 23:50:43 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/28 08:25:37 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	change_value(t_list *env, char *key, char *value)
{
	while (env)
	{
		if (!ft_strcmp(((t_env *)env->content)->key, key))
		{
			free(((t_env *)env->content)->value);
			((t_env *)env->content)->value = value;
			free(((t_env *)env->content)->full);
			((t_env *)env->content)->full = ft_strjoinfree2(key, \
				ft_strjoin("=", value));
			((t_env *)env->content)->export_version = ft_strjoin(\
				"declare -x ", key);
			((t_env *)env->content)->export_version = ft_strjoinfree(\
				((t_env *)env->content)->export_version, "=\"");
			((t_env *)env->content)->export_version = ft_strjoinfree(\
				((t_env *)env->content)->export_version, value);
			((t_env *)env->content)->export_version = ft_strjoinfree(\
				((t_env *)env->content)->export_version, "\"");
			((t_env *)env->content)->is_just_exported = 0;
			return (1);
		}
		env = env->next;
	}
	return (0);
}

void	add_env(t_shell *shell, char *key, char *value)
{
	ft_lstadd_back(&shell->env, ft_lstnew((void *)envnew(key, value, \
		ft_strjoin(key, ft_strjoin("=", value)), 0)));
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

t_env	*envnew(char *key, char *value, char *env_i, int is_just_exported)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->full = ft_strdup(env_i);
	new->is_just_exported = is_just_exported;
	new->export_version = ft_strdup("declare -x ");
	new->export_version = ft_strjoinfree(new->export_version, key);
	if (!is_just_exported)
	{
		new->export_version = ft_strjoinfree(new->export_version, "=\"");
		new->export_version = ft_strjoinfree(new->export_version, value);
		new->export_version = ft_strjoinfree(new->export_version, "\"");
	}
	else if (is_just_exported == 1)
		new->export_version = ft_strjoinfree(new->export_version, "=\"\"");
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
			ft_lstnew((void *)envnew(key, value, envp[i], 0)));
		i++;
	}
	return (1);
}

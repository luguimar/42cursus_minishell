/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:58:44 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/28 08:37:19 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_list	*ft_envsort(t_list *env)
{
	t_list	*sorted;
	t_list	*tmp;
	t_list	*tmp2;
	t_env	*tmp_env;
	t_env	*tmp_env2;

	sorted = NULL;
	tmp = env;
	while (tmp)
	{
		tmp2 = env;
		while (tmp2)
		{
			tmp_env = (t_env *)tmp->content;
			tmp_env2 = (t_env *)tmp2->content;
			if (ft_strcmp(tmp_env->key, tmp_env2->key) < 0)
				ft_lstadd_back(&sorted, ft_lstnew(tmp_env2));
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
	return (sorted);
}

static void	print_sorted_env(t_list *env)
{
	t_list	*exported;
	t_list	*tmp;

	exported = ft_envsort(env);
	tmp = exported;
	while (exported)
	{
		ft_putstr_fd(((t_env *)exported->content)->export_version, 1);
		ft_putstr_fd("\n", 1);
		exported = exported->next;
	}
	while (tmp)
		ft_lstdelone(&tmp, tmp, free_env);
}

int	ft_export(char **args, t_shell *shell)
{
	int		i;
	int		j;
	t_env	*new_env;
	char	*tmp;
	char	*tmp2;

	i = 0;
	if (ft_matrixlen((void **)args) == 1)
	{
		print_sorted_env(shell->env);
		shell->exit_status = 0;
		free_array_of_strings(args);
		return (1);
	}
	while (args[++i])
	{
		j = 0;
		while (!ft_is_special_char(args[i][j]))
			j++;
		if (args[i][j] == '=' && args[i][j + 1] != '\0')
		{
			tmp = ft_substr(args[i], 0, j);
			tmp2 = ft_substr(args[i], j + 1, ft_strlen(args[i]) - j - 1);
			if (!change_value(shell->env, tmp, tmp2))
			{
				new_env = envnew(ft_substr(args[i], 0, j), ft_substr \
					(args[i], j + 1, ft_strlen(args[i]) - j - 1), args[i], 0);
				free(tmp2);
			}
			free(tmp);
		}
		else if (args[i][j] != '=' && args[i][j] != '\0' \
		&& ft_is_special_char(args[i][j]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			shell->exit_status = 1;
			free_array_of_strings(args);
			return (1);
		}
		else if (args[i][j] == '=' && args[i][j + 1] == '\0')
		{
			tmp = ft_substr(args[i], 0, j);
			tmp2 = ft_strdup("");
			if (!change_value(shell->env, tmp, tmp2))
			{
				new_env = envnew(ft_substr(args[i], 0, j), ft_strdup(""), \
					args[i], 1);
				free(tmp2);
			}
			free(tmp);
		}
		else
			new_env = envnew(ft_strdup(args[i]), ft_strdup(""), args[i], 2);
	}
	shell->exit_status = 0;
	if (new_env)
		ft_lstadd_back(&shell->env, ft_lstnew(new_env));
	free_array_of_strings(args);
	return (1);
}

int	ft_env(char **args, t_shell *shell)
{
	t_list	*tmp;

	if (ft_matrixlen((void **)args) > 1)
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		free_array_of_strings(args);
		shell->exit_status = 1;
		return (1);
	}
	tmp = shell->env;
	while (tmp)
	{
		ft_putstr_fd(((t_env *)tmp->content)->full, 1);
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
	shell->exit_status = 0;
	free_array_of_strings(args);
	return (1);
}

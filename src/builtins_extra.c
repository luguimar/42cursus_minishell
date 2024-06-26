/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:58:44 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/21 08:11:47 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_envsort(t_list *env)
{
	t_list	*tmp;
	int		swapped;

	tmp = env;
	swapped = 0;
	while (tmp->next)
	{
		if (ft_strcmp(((t_env *)tmp->content)->key, \
			((t_env *)tmp->next->content)->key) > 0)
		{
			ft_lstswap(tmp, tmp->next);
			swapped = 1;
		}
		tmp = tmp->next;
		if (!tmp->next && swapped)
		{
			tmp = env;
			swapped = 0;
		}
	}
}

static void	print_sorted_env(t_list *env)
{
	t_list	*exported;
	t_list	*tmp;

	exported = ft_lstdup(env);
	ft_envsort(exported);
	if (!exported)
	{
		ft_putstr_fd("failed to sort env", 2);
		return ;
	}
	tmp = exported;
	while (exported)
	{
		ft_putstr_fd(((t_env *)exported->content)->export_version, 1);
		ft_putstr_fd("\n", 1);
		exported = exported->next;
	}
	ft_lstclear(&tmp, &ft_nothing);
}

int	ft_export(char **args, t_shell *shell)
{
	int		i;
	int		j;
	t_env	*new_env;
	char	*tmp;
	char	*tmp2;

	i = 0;
	new_env = NULL;
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
		if (ft_isdigit(args[i][j]) || args[i][j] == '=')
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			shell->exit_status = 1;
			free_array_of_strings(args);
			return (1);
		}
		while (!ft_is_special_char(args[i][j]))
			j++;
		if (args[i][j] == '=' && args[i][j + 1] != '\0')
		{
			tmp = ft_substr(args[i], 0, j);
			tmp2 = ft_substr(args[i], j + 1, ft_strlen(args[i]) - j - 1);
			if (!change_value(shell->env, tmp, tmp2))
				new_env = envnew(tmp, tmp2, args[i], 0);
			else
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
				new_env = envnew(tmp, tmp2, args[i], 1);
			else
				free(tmp);
		}
		else if (get_env_value(shell->env, args[i]) == NULL)
		{
			tmp = ft_strdup(args[i]);
			tmp2 = ft_strdup("");
			new_env = envnew(tmp, tmp2, args[i], 2);
		}
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
		if (((t_env *)tmp->content)->is_just_exported == 0)
		{
			ft_putstr_fd(((t_env *)tmp->content)->full, 1);
			ft_putstr_fd("\n", 1);
		}
		tmp = tmp->next;
	}
	shell->exit_status = 0;
	free_array_of_strings(args);
	return (1);
}

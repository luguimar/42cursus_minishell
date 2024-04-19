/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 05:09:34 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/19 05:37:13 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_cd_case_dash_aux(char **path)
{
	free(*path);
	*path = NULL;
}

int	ft_cd_case_dash(t_shell *shell, char **args)
{
	char	*path;
	char	*pwd;

	if (!get_env_value(shell->env, "OLDPWD"))
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd("OLDPWD not set\n", 2);
		return (free_array_of_strings(args), 1);
	}
	if (chdir(get_env_value(shell->env, "OLDPWD")) == -1)
	{
		free_array_of_strings(args);
		ft_putstr_fd("cd: ", 2);
		return (perror(get_env_value(shell->env, "OLDPWD")), 1);
	}
	path = ft_strdup(get_env_value(shell->env, "OLDPWD"));
	pwd = getcwd(NULL, 0);
	change_value(shell->env, "OLDPWD", pwd);
	if (get_env_value(shell->env, "PWD"))
		change_value(shell->env, "PWD", path);
	else
		ft_cd_case_dash_aux(&path);
	if (path)
		printf("%s\n", path);
	return (free_array_of_strings(args), 1);
}

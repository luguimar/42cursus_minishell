/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:10:31 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/17 13:17:31 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cd_case_dotdot(t_shell *shell, char ***args)
{
	char	*path;
	int		i;

	path = get_env_value(shell->env, "PWD");
	i = ft_strlen(path) - 1;
	while (i >= 0 && path[i] != '/')
		i--;
	path = ft_substr(path, 0, i);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		perror(path);
		free(path);
		free_array_of_strings(*args);
		return (1);
	}
	change_value(shell->env, "OLDPWD", \
		ft_strdup(get_env_value(shell->env, "PWD")));
	change_value(shell->env, "PWD", path);
	free_array_of_strings(*args);
	return (1);
}

int	ft_cd_aux_extra(t_shell *shell, char ***args)
{
	if (ft_strcmp((*args)[1], ".") == 0)
	{
		free_array_of_strings(*args);
		return (1);
	}
	else if (ft_strcmp((*args)[1], "..") == 0)
		return (ft_cd_case_dotdot(shell, args));
	else if (ft_strcmp((*args)[1], "--") == 0)
		return (ft_cd_case_double_dash(shell, args));
	return (0);
}

int	ft_cd_case_slash(t_shell *shell, char ***args)
{
	if (chdir((*args)[1]) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		perror((*args)[1]);
		free_array_of_strings(*args);
		return (1);
	}
	change_value(shell->env, "OLDPWD", \
		ft_strdup(get_env_value(shell->env, "PWD")));
	change_value(shell->env, "PWD", ft_strdup((*args)[1]));
	free_array_of_strings(*args);
	return (1);
}

int	ft_cd_case_tild(t_shell *shell, char ***args)
{
	char	*path;

	path = ft_strjoin(get_env_value(shell->env, "HOME"), (*args)[1] + 1);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		perror(path);
		free(path);
		free_array_of_strings(*args);
		return (1);
	}
	change_value(shell->env, "OLDPWD", \
		ft_strdup(get_env_value(shell->env, "PWD")));
	change_value(shell->env, "PWD", path);
	free_array_of_strings(*args);
	return (1);
}

int	ft_cd_aux(char **args, t_shell *shell)
{
	char	*path;

	if (ft_strcmp(args[1], "-") == 0)
	{
		if (chdir(get_env_value(shell->env, "OLDPWD")) == -1)
		{
			ft_putstr_fd("cd: ", 2);
			perror(get_env_value(shell->env, "OLDPWD"));
			return (1);
		}
		path = ft_strdup(get_env_value(shell->env, "OLDPWD"));
		change_value(shell->env, "OLDPWD", \
			ft_strdup(get_env_value(shell->env, "PWD")));
		change_value(shell->env, "PWD", path);
		printf("%s\n", path);
		free_array_of_strings(args);
		return (1);
	}
	else if (args[1][0] == '~')
		return (ft_cd_case_tild(shell, &args));
	else if (args[1][0] == '/')
		ft_cd_case_slash(shell, &args);
	else
		return (ft_cd_aux_extra(shell, &args));
	return (0);
}

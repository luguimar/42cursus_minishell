/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:16:47 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/28 09:03:42 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cd_case_double_dash(t_shell *shell, char ***args)
{
	if (!get_env_value(shell->env, "HOME"))
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd("HOME not set\n", 2);
		free_array_of_strings(*args);
		return (1);
	}
	if (chdir(get_env_value(shell->env, "HOME")) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(get_env_value(shell->env, "HOME"), 2);
		free_array_of_strings(*args);
		return (1);
	}
	if (get_env_value(shell->env, "PWD") && get_env_value \
	(shell->env, "OLDPWD"))
		change_value(shell->env, "OLDPWD", \
			ft_strdup(get_env_value(shell->env, "PWD")));
	if (get_env_value(shell->env, "PWD") && get_env_value \
	(shell->env, "HOME"))
		change_value(shell->env, "PWD", \
			ft_strdup(get_env_value(shell->env, "HOME")));
	free_array_of_strings(*args);
	shell->exit_status = 0;
	return (1);
}

char	*ft_strremoveprev(char *str, char *old, char *new)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;
	char	*tmp4;
	int		i;

	tmp = ft_strnstr(str, old, ft_strlen(str));
	tmp2 = ft_substr(str, 0, tmp - str);
	i = ft_strlen(tmp2) - 1;
	while (tmp2[i] != '/')
		i--;
	tmp3 = ft_substr(tmp2, 0, i);
	tmp4 = ft_strjoinfree(ft_strjoinfree(tmp3, new), tmp + ft_strlen(old));
	free(str);
	free(tmp2);
	return (tmp4);
}

char	*ft_strreplace(char *str, char *old, char *new)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;
	char	*tmp4;

	tmp = ft_strnstr(str, old, ft_strlen(str));
	tmp2 = ft_substr(str, 0, tmp - str);
	tmp3 = ft_strjoin(tmp2, new);
	tmp4 = ft_strjoin(tmp3, tmp + ft_strlen(old));
	free(str);
	free(tmp2);
	free(tmp3);
	return (tmp4);
}

char	*ft_cd_check_for_dots(char *args)
{
	while (ft_strnstr(args, "/./", ft_strlen(args)))
		args = ft_strreplace(args, "/./", "/");
	while (ft_strnstr(args, "/./\0", ft_strlen(args)))
		args = ft_strreplace(args, "/./", "/");
	while (ft_strnstr(args, "/../", ft_strlen(args)))
		args = ft_strremoveprev(args, "/../", "/");
	if (ft_strnstr(args, "/..\0", ft_strlen(args)))
		args = ft_strremoveprev(args, "/..", "");
	return (args);
}

int	ft_cd_normal(char **args, t_shell *shell)
{
	char	*path;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	path = ft_strjoin(pwd, "/");
	path = ft_strjoinfree(path, args[1]);
	path = ft_cd_check_for_dots(path);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		perror(path);
		free(path);
		free_array_of_strings(args);
		shell->exit_status = 1;
		return (1);
	}
	if (get_env_value(shell->env, "OLDPWD"))
		change_value(shell->env, "OLDPWD", pwd);
	else
		free(pwd);
	if (get_env_value(shell->env, "PWD"))
		change_value(shell->env, "PWD", path);
	else
		free(path);
	free_array_of_strings(args);
	shell->exit_status = 0;
	return (1);
}

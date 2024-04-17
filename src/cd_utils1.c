/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:16:47 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/17 13:17:25 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cd_case_double_dash(t_shell *shell, char ***args)
{
	if (chdir(get_env_value(shell->env, "HOME")) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		perror(get_env_value(shell->env, "HOME"));
		free_array_of_strings(*args);
		return (1);
	}
	change_value(shell->env, "OLDPWD", \
		ft_strdup(get_env_value(shell->env, "PWD")));
	change_value(shell->env, "PWD", \
		ft_strdup(get_env_value(shell->env, "HOME")));
	free_array_of_strings(*args);
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
	tmp4 = ft_strjoinfree(ft_strjoin(tmp3, new), tmp + ft_strlen(old));
	free(tmp2);
	free(tmp3);
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
	free(tmp2);
	free(tmp3);
	return (tmp4);
}

char	*ft_cd_check_for_dots(char **args, t_shell *shell)
{
	char	*tmp;

	tmp = ft_strjoinfree2(get_env_value(shell->env, "PWD"), \
		ft_strjoin("/", args[1]));
	while (ft_strnstr(tmp, "/./", ft_strlen(tmp)))
		tmp = ft_strreplace(tmp, "/./", "/");
	while (ft_strnstr(tmp, "/../", ft_strlen(tmp)))
		tmp = ft_strremoveprev(tmp, "/../", "/");
	if (ft_strnstr(tmp, "/..\0", ft_strlen(tmp)))
		tmp = ft_strremoveprev(tmp, "/..", "");
	return (tmp);
}

int	ft_cd_normal(char **args, t_shell *shell)
{
	char	*path;

	path = ft_cd_check_for_dots(args, shell);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		perror(path);
		free(path);
		free_array_of_strings(args);
		return (1);
	}
	change_value(shell->env, "OLDPWD", \
		ft_strdup(get_env_value(shell->env, "PWD")));
	change_value(shell->env, "PWD", path);
	free_array_of_strings(args);
	return (1);
}

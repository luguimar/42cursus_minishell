/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 05:09:37 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/17 11:31:24 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strremoveprev(char *str, char *old)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;
	char	*tmp4;

	tmp = ft_strnstr(str, old, ft_strlen(str));
	tmp2 = ft_substr(str, 0, tmp - str);
	tmp3 = ft_strjoin(tmp2, tmp + ft_strlen(old));
	free(tmp2);
	return (tmp3);
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
	char	*path;
	char	*tmp;

	tmp = ft_strjoinfree2(get_env_value(shell->env, "PWD"), \
		ft_strjoin("/", args[1]));
	while (ft_strnstr(tmp, "/./", ft_strlen(tmp)))
		tmp = ft_strreplace(tmp, "/./", "/");
	while (ft_strnstr(tmp, "/../", ft_strlen(tmp)))
		tmp = ft_strremoveprev(tmp, "/../");
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
}

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

int	ft_cd(char **args, t_shell *shell)
{
	auto char *path;
	if (ft_matrixlen((void **)args) > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		free_array_of_strings(args);
		return (1);
	}
	if (ft_cd_aux(args, shell))
		return (1);
	if (ft_matrixlen((void **)args) == 1)
		path = ft_strdup(get_env_value(shell->env, "HOME"));
	else
		return (ft_cd_normal(args, shell));
	return (1);
}

int	exec_builtin(char **args, t_shell *shell)
{
	char	**new_args;

	new_args = ft_splitquote(args[0], ' ');
	if (ft_strcmp(new_args[0], "cd") == 0)
		return (ft_cd(new_args, shell));
	/*else if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(new_args, shell));
	else if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(new_args, shell));
	else if (ft_strcmp(args[0], "exit") == 0)
		return (ft_exit(new_args, shell));
	else if (ft_strcmp(args[0], "export") == 0)
		return (ft_export(new_args, shell));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd(new_args, shell));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(new_args, shell));*/
	return (0);
}

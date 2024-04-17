/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 05:09:37 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/17 07:20:55 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cd_case_dotdot(char **path, t_shell *shell, char ***args)
{
	
}

int	ft_cd_aux_extra(char **path, t_shell *shell, char ***args)
{
	if (ft_strcmp((*args)[1], ".") == 0)
	{
		free_array_of_strings(*args);
		return (1);
	}
	else if (ft_strcmp((*args)[1], "..") == 0)
		ft_cd_case_dotdot(path, shell, args);
}

int	ft_cd_case_slash(char **path, t_shell *shell, char ***args)
{
	if (chdir((*args)[1]) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		perror((*args)[1]);
		free(path);
		return (1);
	}
	change_value(shell->env, "OLDPWD", \
		ft_strdup(get_env_value(shell->env, "PWD")));
	change_value(shell->env, "PWD", ft_strdup((*args)[1]));
	free_array_of_strings(*args);
	return (1);
}

int	ft_cd_case_tild(char **path, t_shell *shell, char ***args)
{
	*path = ft_strjoin(get_env_value(shell->env, "HOME"), (*args)[1] + 1);
	if (chdir(*path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		perror(*path);
		free(path);
		return (1);
	}
	change_value(shell->env, "OLDPWD", \
		ft_strdup(get_env_value(shell->env, "PWD")));
	change_value(shell->env, "PWD", *path);
	free_array_of_strings(*args);
	return (1);
}

int	ft_cd_aux(char **args, t_shell *shell)
{
	auto char *path;
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
		return (ft_cd_case_tild(&path, shell, &args));
	else if (args[1][0] == '/')
		ft_cd_case_slash(&path, shell, &args);
	else
		ft_cd_aux_extra(&path, shell, &args);
	return (0);
}

int	ft_cd(char **args, t_shell *shell)
{
	auto char *path;
	if (ft_matrixlen((void **)args) > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	if (ft_cd_aux(args, shell))
		return (0);
	if (ft_matrixlen((void **)args) == 1)
		path = ft_strdup(get_env_value(shell->env, "HOME"));
	else
		path = ft_strjoin(get_env_value(shell->env, "PWD"), args[1]);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		perror(path);
		return (1);
	}
	change_value(shell->env, "OLDPWD", \
		ft_strdup(get_env_value(shell->env, "PWD")));
	change_value(shell->env, "PWD", ft_strdup(path));
	if (ft_matrixlen((void **)args) == 1)
		free(path);
	free_array_of_strings(args);
	return (0);
}

int	exec_builtin(char **args, t_shell *shell)
{
	char	**new_args;

	new_args = ft_splitquote(args[0], ' ');
	free_array_of_strings(args);
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(new_args, shell));
	else if (ft_strcmp(args[0], "echo") == 0)
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
		return (ft_unset(new_args, shell));
	return (0);
}

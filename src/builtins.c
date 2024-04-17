/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 05:09:37 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/17 20:30:31 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_unset(char **args, t_shell *shell)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (ft_isalpha(args[i][0]) || args[i][0] == '_')
			delete_env_node(&shell->env, args[i]);
		else
		{
			ft_putstr_fd("unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		i++;
	}
	free_array_of_strings(args);
	return (1);
}

static int	ft_exit(char **args, t_shell *shell)
{
	int	i;

	i = 0;
	if (ft_matrixlen((void **)args) > 1)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		free_array_of_strings(args);
		return (1);
	}
	free_everything(shell);
	free_array_of_strings(args);
	exit(0);
}

static int	ft_pwd(char **args, t_shell *shell)
{
	char	*pwd;

	if (ft_matrixlen((void **)args) > 1)
	{
		ft_putstr_fd("pwd: too many arguments\n", 2);
		free_array_of_strings(args);
		return (1);
	}
	pwd = get_env_value(shell->env, "PWD");
	ft_putendl_fd(pwd, 1);
	free_array_of_strings(args);
	return (1);
}

static int	ft_cd(char **args, t_shell *shell)
{
	if (ft_matrixlen((void **)args) > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		free_array_of_strings(args);
		return (1);
	}
	if (!args[1])
		return (ft_cd_case_double_dash(shell, &args));
	if (ft_cd_aux(args, shell))
		return (1);
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
	else if (ft_strcmp(new_args[0], "pwd") == 0)
		return (ft_pwd(new_args, shell));
	else if (ft_strcmp(new_args[0], "exit") == 0)
		return (ft_exit(new_args, shell));
	else if (ft_strcmp(new_args[0], "unset") == 0)
		return (ft_unset(new_args, shell));
	else if (ft_strcmp(new_args[0], "env") == 0)
		return (ft_env(new_args, shell));
	/*else if (ft_strcmp(new_args[0], "export") == 0)
		return (ft_export(new_args, shell));
	else if (ft_strcmp(new_args[0], "echo") == 0)
		return (ft_echo(new_args, shell));*/
	free_array_of_strings(new_args);
	return (0);
}
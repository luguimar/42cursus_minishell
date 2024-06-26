/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 05:09:37 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/28 19:09:37 by luguimar         ###   ########.fr       */
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
		i++;
	}
	shell->exit_status = 0;
	free(shell->env_array);
	shell->env_array = env_to_array(shell->env);
	free_array_of_strings(args);
	return (1);
}

int	ft_exit(char **args, t_shell *shell, char **old_args)
{
	int	return_value;

	return_value = 0;
	if (args && ft_matrixlen((void **)args) > 2)
	{
		shell->exit_status = 1;
		ft_putstr_fd("exit: too many arguments\n", 2);
		free_array_of_strings(args);
		return (1);
	}
	free_everything(shell);
	printf("exit\n");
	if (args && args[1] && ft_isint(args[1]))
		return_value = ft_atoi(args[1]);
	else if (args && args[1])
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return_value = 2;
	}
	if (args)
		free_array_of_strings(args);
	if (old_args)
		free_array_of_strings(old_args);
	exit(return_value);
}

static int	ft_pwd(char **args, t_shell *shell)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		ft_putendl_fd(pwd, 1);
		free(pwd);
	}
	free_array_of_strings(args);
	shell->exit_status = 0;
	return (1);
}

static int	ft_cd(char **args, t_shell *shell)
{
	if (ft_matrixlen((void **)args) > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		free_array_of_strings(args);
		shell->exit_status = 1;
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

int	exec_builtin(char **args, t_shell *shell, int ispipex)
{
	char	**new_args;

	if (ispipex)
	{
		new_args = ft_dup_array_of_strings(args);
		if (!new_args)
			return (-1);
	}
	else
	{
		new_args = ft_splitquote_nulls(args[0], ' ');
		if (!new_args)
			return (-1);
		if (ft_strcmp(new_args[0], "cd") && ft_strcmp \
		(new_args[0], "pwd") && ft_strcmp(new_args[0], \
		"exit") && ft_strcmp(new_args[0], "unset") && \
		ft_strcmp(new_args[0], "env") && ft_strcmp \
		(new_args[0], "export") && ft_strcmp(new_args[0], "echo"))
		{
			free_array_of_strings(new_args);
			return (0);
		}
		free_array_of_strings(new_args);
		redirects_handler(shell, 0, NULL, args, 0);
		new_args = ft_splitquote(args[0], ' ');
	}
	if (!new_args)
		return (-1);
	if (!new_args[0])
	{
		free_array_of_strings(new_args);
		return (0);
	}
	if (ft_strcmp(new_args[0], "cd") == 0)
		return (ft_cd(new_args, shell));
	else if (ft_strcmp(new_args[0], "pwd") == 0)
		return (ft_pwd(new_args, shell));
	else if (ft_strcmp(new_args[0], "exit") == 0)
		return (ft_exit(new_args, shell, args));
	else if (ft_strcmp(new_args[0], "unset") == 0)
		return (ft_unset(new_args, shell));
	else if (ft_strcmp(new_args[0], "env") == 0)
		return (ft_env(new_args, shell));
	else if (ft_strcmp(new_args[0], "export") == 0)
		return (ft_export(new_args, shell));
	else if (ft_strcmp(new_args[0], "echo") == 0)
		return (ft_echo(new_args));
	free_array_of_strings(new_args);
	return (0);
}

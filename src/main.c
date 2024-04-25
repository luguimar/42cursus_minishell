/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:26:09 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/25 04:34:01 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_everything(t_shell *shell)
{
	t_list	*tmp;
	t_list	*tmp2;

	tmp = shell->env;
	tmp2 = shell->env;
	while (tmp)
	{
		tmp = tmp2;
		if (tmp2)
		{
			tmp2 = tmp2->next;
			free_env(tmp->content);
			free(tmp);
		}
	}
	free(shell->env_array);
	free(shell->input);
}

static int	check_args(char **args)
{
	int	i;
	int	j;

	i = 0;
	while (args[i])
	{
		j = 0;
		while ((args[i][j] == ' ' || args[i][j] == '\t' || args[i][j] == '\n' \
		|| args[i][j] == '\v' || args[i][j] == '\f' || args[i][j] == '\r') \
		&& args[i][j])
			j++;
		if (!args[i][j])
		{
			free_array_of_strings(args);
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n" \
			, 2);
			return (0);
		}
		i++;
	}
	return (1);
}

int	minishell(t_shell *shell)
{
	char	**args;

	if (shell->input[0] == '|' || shell->input[ft_strlen \
	(shell->input) - 1] == '|')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (0);
	}
	args = ft_split_if_not_in_quote(shell->input, '|');
	if (args == NULL)
		return (1);
	if (!check_args(args))
		return (0);
	shell->arg_count = ft_matrixlen((void **) args);
	if (ft_matrixlen((void **) args) == 1)
	{
		if (exec_builtin(args, shell))
		{
			free_array_of_strings(args);
			return (0);
		}
	}
	pipex(shell->arg_count, args, shell);
	free_array_of_strings(args);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell				shell;

	(void)argc;
	(void)argv;
	shell.env = NULL;
	shell.input = NULL;
	shell.env = NULL;
	shell.exit_status = 0;
	env_to_list(&shell, envp);
	shell.env_array = env_to_array(shell.env);
	while (1)
	{
		shell.input = readline("minishell$>");
		if (shell.input == NULL)
			break ;
		minishell(&shell);
		free(shell.input);
	}
	return (0);
}

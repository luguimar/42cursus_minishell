/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:26:09 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/17 05:45:47 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_everything(t_shell *shell)
{
	t_list	*tmp;

	while (shell->env)
	{
		tmp = shell->env;
		shell->env = shell->env->next;
		free(((t_env *)tmp->content)->full);
		free(tmp->content);
		free(tmp);
	}
	free(shell->env_array);
}

int	minishell(t_shell *shell)
{
	auto char **args;
	auto int cid;
	args = ft_split_if_not_in_quote(shell->input, '|');
	if (args == NULL)
		return (1);
	if (ft_matrixlen((void **) args) == 1)
	{
		if (exec_builtin(args, shell))
		{
			free_array_of_strings(args);
			return (0);
		}
	}
	cid = fork();
	if (cid == -1)
	{
		ft_putstr_fd("fork: ", 2);
		perror(NULL);
		return (1);
	}
	if (cid == 0)
		pipex(ft_matrixlen((void **) args), args, env_to_array(shell->env));
	else
		waitpid(cid, NULL, 0);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.env = NULL;
	shell.input = NULL;
	shell.env = NULL;
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

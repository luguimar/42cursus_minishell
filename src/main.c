/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:26:09 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/23 18:49:37 by jduraes-         ###   ########.fr       */
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

int	minishell(t_shell *shell)
{
	char	**args;
	//int		cid;

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
	/*cid = fork();
	if (cid == -1)
		return (ft_putstr_fd("fork: ", 2), perror(NULL), 1);
	if (cid == 0)*/
	pipex(ft_matrixlen((void **) args), args, shell);
	/*else
		waitpid(cid, NULL, 0);*/
	free_array_of_strings(args);
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
	shell.exit_status = 0;
	env_to_list(&shell, envp);
	shell.env_array = env_to_array(shell.env);
	while (1)
	{
		sigset(1);
		shell.input = readline("minishell$>");
		if (shell.input == NULL)
			break ;
		minishell(&shell);
		free(shell.input);
	}
	return (0);
}

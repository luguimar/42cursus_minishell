/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:26:09 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/16 01:41:58 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	minishell(t_shell shell)
{
	char	**args;
	int		cid;

	cid = fork();
	if (cid == -1)
	{
		ft_putstr_fd("fork: ", 2);
		perror(NULL);
		return (1);
	}
	if (cid == 0)
	{
		args = ft_split_if_not_in_quote(input, '|');
		if (args == NULL)
			return (1);
		pipex(ft_matrixlen((void **) args), args, envp);
	}
	else
		waitpid(cid, NULL, 0);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	while (1)
	{
		shell.input = readline("minishell$>");
		if (shell.input == NULL)
			break ;
		minishell(shell);
		/*if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		ft_putstr_fd(input, 1);
		ft_putchar_fd('\n', 1);*/
		free(shell.input);
	}
	return (0);
}

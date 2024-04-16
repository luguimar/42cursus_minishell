/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:26:09 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/17 00:36:57 by luguimar         ###   ########.fr       */
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
}

int	minishell(t_shell *shell)
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
		args = ft_split_if_not_in_quote(shell->input, '|');
		if (args == NULL)
			return (1);
		pipex(ft_matrixlen((void **) args), args, env_to_array(shell->env));
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
	shell.env = NULL;
	env_to_list(&shell, envp);
	while (1)
	{
		shell.input = readline("minishell$>");
		if (shell.input == NULL)
			break ;
		minishell(&shell);
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

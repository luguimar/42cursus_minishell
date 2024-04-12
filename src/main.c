/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:26:09 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/12 22:03:43 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	minishell(char *input, char **envp)
{
	char	**args;

	args = ft_splitquote(input, ' ');
	if (args == NULL)
		return (1);
	pipex(ft_matrixlen((void **) args), args, envp);
	if (args == NULL)
		return (1);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;

	(void)argc;
	(void)argv;
	(void)envp;
	while (1)
	{
		input = readline("minishell$>");
		if (input == NULL)
			break ;
		minishell(input, envp);
		/*if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		ft_putstr_fd(input, 1);
		ft_putchar_fd('\n', 1);*/
		free(input);
	}
	return (0);
}

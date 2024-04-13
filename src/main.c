/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:26:09 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/13 04:06:32 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**matrix_dup(char **matrix)
{
	char	**new_matrix;
	int		i;

	i = 0;
	while (matrix[i])
		i++;
	new_matrix = (char **)malloc(sizeof(char *) * (i + 1));
	if (new_matrix == NULL)
		return (NULL);
	i = 0;
	while (matrix[i])
	{
		new_matrix[i] = ft_strdup(matrix[i]);
		if (new_matrix[i] == NULL)
		{
			free_array_of_strings(new_matrix);
			return (NULL);
		}
		i++;
	}
	new_matrix[i] = NULL;
	return (new_matrix);
}

int	minishell(char *input, char **envp)
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
		args = ft_splitquote(input, '|');
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
	char	*input;
	char	**envp_dup;

	(void)argc;
	(void)argv;
	envp_dup = matrix_dup(envp);
	while (1)
	{
		input = readline("minishell$>");
		if (input == NULL)
			break ;
		minishell(input, envp_dup);
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

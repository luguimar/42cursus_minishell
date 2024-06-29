/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:26:09 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/29 19:49:07 by jduraes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>

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
	if (shell->env_array)
		free(shell->env_array);
	if (shell->input)
		free(shell->input);
	if(shell->heredocs)
		free(shell->heredocs);
	free(shell);
	rl_clear_history();
}

int	arg_check(char **input, t_shell *shell)
{
	char	**new;
	int	i;

	i = -1;
	(void)new;
	(void)shell;
	*input = ft_strtrimfree(*input, " \t\n\v\f\r");
	while ((*input)[++i])
	{
		if (is_c_not_in_quotes(*input, i, '|') && (*input)[i + 1] == '|')
		{
			ft_putstr_fd("minishell: no suport for \"||\"\n", 2);
            return (0);
		}
	}
	if (inquote(*input, i - 1))
	{
		ft_putstr_fd("minishell: no suport for open quotes\n", 2);
        return (0);
	}
//	new = ft_split_if_not_in_quote(input, '|');
	return (1);
}

int	minishell(t_shell *shell)
{
	char	**args;
	int		i;
	int		orig_stdout;
	int		orig_stdin;

	i = -1;
	if (!arg_check(&(shell->input), shell))
		return (1);
	args = ft_split_if_not_in_quote(shell->input, '|');
	if (args == NULL)
		return (1);
	shell->arg_count = ft_matrixlen((void **) args);
	shell->heredocs = malloc(sizeof(int) * (shell->arg_count + 1));
	if (shell->heredocs == NULL)
		return (1);
	shell->heredocs[shell->arg_count] = -1;
	while (args[++i])
	{
		heredocs(args[i], i, shell);
		expand(&args[i], shell, -1, 0);
	}
	if (ft_matrixlen((void **) args) == 1)
	{
		orig_stdout = dup(STDOUT_FILENO);
		orig_stdin = dup(STDIN_FILENO);
		if (exec_builtin(args, shell, 0))
		{
			dup2(orig_stdout, STDOUT_FILENO);
			dup2(orig_stdin, STDIN_FILENO);
			close(orig_stdout);
			close(orig_stdin);
			return (free_array_of_strings(args), 0);
		}
		else
		{
			dup2(orig_stdout, STDOUT_FILENO);
			dup2(orig_stdin, STDIN_FILENO);
			close(orig_stdout);
			close(orig_stdin);
		}
	}
	pipex(shell->arg_count, args, shell);
	heredoc_unlink(shell);
	free_array_of_strings(args);
	free(shell->heredocs);
	return (0);
}

t_shell	*initialize(void)
{
	t_shell	*shell;

	shell = ft_calloc(1, sizeof(t_shell));
	return (shell);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell				*shell;

	(void)argc;
	(void)argv;
	shell = initialize();
	env_to_list(shell, envp);
	shell->env_array = env_to_array(shell->env);
	while (1)
	{
		signal(SIGINT, main_handler);
		signal(SIGQUIT, SIG_IGN);
		shell->input = readline("minishell$> ");
		if (shell->input == NULL)
			ft_exit(NULL, shell, NULL);
		add_history(shell->input);
		minishell(shell);
		free(shell->input);
	}
	return (0);
}

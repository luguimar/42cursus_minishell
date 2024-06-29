/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:26:09 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/28 19:29:37 by luguimar         ###   ########.fr       */
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
	free(shell->env_array);
	free(shell->input);
	rl_clear_history();
}

int	minishell(t_shell *shell)
{
	char	**args;
	int		i;
	int		orig_stdout;
	int		orig_stdin;

	i = -1;
	shell->input = ft_strtrim(shell->input, " \t\n\v\f\r");
	if (shell->input[0] == '|' || (ft_strlen(shell->input) != 0 && \
	shell->input[ft_strlen(shell->input) - 1] == '|'))
		return (ft_putstr_fd \
		("minishell: syntax error near unexpected token `|'\n", 2), 0);
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
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell				shell;

	(void)argc;
	(void)argv;
	shell.env = NULL;
	shell.input = NULL;
	shell.env_array = NULL;
	shell.exit_status = 0;
	env_to_list(&shell, envp);
	shell.env_array = env_to_array(shell.env);
	while (1)
	{
		signal(SIGINT, main_handler);
		signal(SIGQUIT, SIG_IGN);
		shell.input = readline("minishell$> ");
		if (shell.input == NULL)
			ft_exit(NULL, &shell, NULL);
		add_history(shell.input);
		minishell(&shell);
		free(shell.input);
	}
	return (0);
}

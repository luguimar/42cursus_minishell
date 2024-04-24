/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 21:18:14 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/24 16:47:53 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	dup2stdout(int *pipefd)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
}

void	dup2stdin(int *pipefd)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
}

int	get_right_path_aux(char **cmd, char **path, int i, char **right_path)
{
	*right_path = ft_strjoin(path[i], "/");
	*right_path = ft_strjoinfree(*right_path, *cmd);
	if (access(*right_path, F_OK) == 0)
	{
		free_array_of_strings(path);
		return (1);
	}
	return (0);
}

void	heredoc(char *limiter)
{
	int		heredoc_fd;
	char	*line;

	heredoc_fd = open("here_doc", O_RDWR | O_TRUNC | O_CREAT, 0664);
	ft_printf("> ");
	line = get_next_line(0);
	while (line && ft_strncmp(line, limiter, ft_strlen(limiter)))
	{
		ft_putstr_fd(line, heredoc_fd);
		free(line);
		ft_printf("> ");
		line = get_next_line(0);
	}
	free(line);
	close (heredoc_fd);
}

void	redirect_files_aux(int cid, int i, t_shell *shell)
{
	if (i != shell->arg_count - 1)
		shell->pids[i] = cid;
	else
	{
		shell->pids[i] = cid;
		while (--i >= 0)
			waitpid(shell->pids[i], NULL, WNOHANG);
		waitpid(cid, &shell->proccess_status, 0);
		if (WIFEXITED(shell->proccess_status))
			shell->exit_status = WEXITSTATUS(shell->proccess_status);
	}
}
/*
char	**last_one(char **argv, char **path, char **envp, int i)
{
	char	**args;
	int		argc;

	(void)argc;
	argc = ft_matrixlen((void **)argv);
	args = ft_splitquote_nulls(argv[i], ' ');
	*path = get_right_path(args, envp, *path);
	//check_error(access(argv[argc - 1], W_OK), argv[argc - 1], args, *path);
	return (args);
}

void	dup2redirect(int *fd, char **argv, t_shell *shell, int i)
{
	fd[0] = open(argv[1], O_RDONLY);
	if (i != 3)
		fd[1] = open(argv[ft_matrixlen((void **)argv) - 1],
				O_WRONLY | O_CREAT | O_TRUNC, 0664);
	else
		fd[1] = open(argv[ft_matrixlen((void **)argv) - 1],
				O_WRONLY | O_CREAT | O_APPEND, 0664);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	if (i == 3)
		redirect_files(-3, argv, shell);
	else
		redirect_files(i, argv, shell);
}*/

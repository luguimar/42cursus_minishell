/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 21:18:14 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/26 20:51:41 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_getdirs(char *cmd)
{
	char	*pwd;
	char	*path;

	pwd = getcwd(NULL, 0);
	if ((cmd[0] == '.' && cmd[1] == '/') || (cmd[0] == '.' \
	&& cmd[1] == '.' && cmd[2] == '/'))
	{
		path = ft_strjoinfree(pwd, "/");
		path = ft_strjoinfree(path, cmd);
		path = ft_cd_check_for_dots(path);
	}
	else
	{
		path = ft_strdup(cmd);
		path = ft_cd_check_for_dots(path);
		free(pwd);
	}
	return (path);
}

void	dup2pipe(int **fds, int i, t_shell *shell, char **args)
{
	int	j;

	j = -1;
	while (++j < i - 1)
	{
		close(fds[j][0]);
		close(fds[j][1]);
	}
	redirects_handler(shell, i, fds, args, 1);
}

int	get_right_path_aux2(char **cmd, char **right_path)
{
	if (cmd && ft_strchr(cmd[0], '/'))
	{
		*right_path = ft_getdirs(cmd[0]);
		if (access(*right_path, F_OK) == 0)
			return (1);
		free(*right_path);
		right_path = NULL;
	}
	return (0);
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

void	redirect_files_aux(int cid, int i, t_shell *shell, int ***fds)
{
	int	j;

	j = i;
	signal(SIGINT, sigint_parent);
	if (j == shell->arg_count - 1)
	{
		while (--j >= 0)
		{
			close((*fds)[j][0]);
			close((*fds)[j][1]);
		}
	}
	shell->pids[i] = cid;
	if (i == shell->arg_count - 1)
	{
		waitpid(cid, &shell->proccess_status, 0);
		while (--i >= 0)
			waitpid(shell->pids[i], NULL, 0);
		if (WIFEXITED(shell->proccess_status))
			shell->exit_status = WEXITSTATUS(shell->proccess_status);
		else if (WIFSIGNALED(shell->proccess_status))
			shell->exit_status = 128 + WTERMSIG(shell->proccess_status);
		if (shell->exit_status == 131)
			ft_putstr_fd("Quit (core dumped)\n", 1);
	}
	unlink(".here_doc");
}
/*
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

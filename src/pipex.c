/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:07:51 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/19 02:28:44 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	exec_command(char *path, t_shell *shell, char **args, int isparent)
{
	if (exec_builtin(args, shell))
	{
		free_everything(shell);
		free_array_of_strings(args);
		if (path)
			free(path);
		exit(0);
	}
	if (!path || !shell->env_array || !args)
	{
		while (--isparent)
			wait(NULL);
		dup2(STDERR_FILENO, STDOUT_FILENO);
		if (args)
			ft_printf("%s: command not found\n", args[0]);
		else
			ft_printf("minishell: '' : command not found\n");
		if (path)
			free(path);
		free_array_of_strings(args);
		exit(127);
	}
	execve(path, args, shell->env_array);
	exit(execve_error(path, args, shell, isparent));
}

void	check_error(int status, char *message, char **args, char *path)
{
	if (status == -1)
	{
		if (args)
			free_array_of_strings(args);
		if (path)
			free(path);
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		perror(message);
		exit(1);
	}
	return ;
}

char	*get_right_path(char **cmd, char **envp, char *right_path)
{
	int		i;
	char	**path;

	i = 0;
	if (!envp || !cmd)
		return (NULL);
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	path = ft_split(envp[i] + 5, ':');
	i = -1;
	while (cmd && path[++i] && *cmd[0] != '/')
	{
		if (get_right_path_aux(cmd, path, i, &right_path))
			return (right_path);
		free(right_path);
	}
	free_array_of_strings(path);
	if (cmd && *cmd[0] == '/' && access(*cmd, F_OK) == 0)
		return (ft_strdup(*cmd));
	return (NULL);
}

void	redirect_files(int i, char *argv[], t_shell *shell)
{
	int		cid;
	int		pipefd[2];
	char	*path;
	char	**args;

	args = NULL;
	path = NULL;
	check_error(pipe(pipefd), "pipe", args, path);
	cid = fork();
	if (cid == 0)
	{
		args = ft_splitquote_nulls(argv[ft_abs_value(i)], ' ');
		path = get_right_path(args, shell->env_array, path);
		//if (i == 2 || i == -3)
		//	check_error(access(argv[1], R_OK), argv[1], args, path);
		dup2stdout(pipefd);
		exec_command(path, shell, args, 1);
	}
	else if (cid == -1)
		check_error(-1, "fork", args, path);
	else
	{
		dup2stdin(pipefd);
		waitpid(cid, NULL, WNOHANG);
	}
}

int	pipex(int argc, char **argv, t_shell *shell)
{
	int		fd[2];
	int		i;
	char	*path;
	char	**args;

	(void)fd;
	i = -1;
	path = NULL;
	//dup2redirect(fd, argv, shell, i);
	while (++i < argc - 1)
		redirect_files(i, argv, shell);
	args = last_one(argv, &path, shell->env_array, i);
	exec_command(path, shell, args, 1);
	return (1);
}

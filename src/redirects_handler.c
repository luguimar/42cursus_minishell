/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 09:09:59 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/28 13:18:03 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_file_name(t_shell *shell, char **args, int i)
{
	int		j;
	char	*file_name;

	i++;
	while ((*args)[i] && is_c_not_in_quotes(*args, i, ' '))
		i++;
	j = i;
	while ((*args)[j] && !is_c_not_in_quotes(*args, j, ' ') && \
	!is_c_not_in_quotes(*args, j, '<') && !is_c_not_in_quotes(*args, j, '>'))
		j++;
	file_name = ft_substr(*args, i, j - i);
	file_name = ft_remove_quotes(file_name);
	file_name = ft_getdirs(file_name);
	if (!file_name)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("malloc failed\n", 2);
		shell->exit_status = 1;
		return (NULL);
	}
	return (file_name);
}

static int	redirect_output(t_shell *shell, char **args, int i, int is_pipex)
{
	int		file_out;
	int		is_append;
	char	*file_name;
	int		arg_nr;

	arg_nr = 0;
	while (args[arg_nr])
		arg_nr++;
	arg_nr = shell->arg_count - arg_nr - 1;
	is_append = 0;
	if (args[0][i + 1] == args[0][i])
	{
		is_append = 1;
		i++;
	}
	file_name = get_file_name(shell, args, i);
	if (file_name == NULL)
		return (-1);
	if (is_append)
	{
		if (is_directory(file_name))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file_name, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			shell->exit_status = 126;
			free(file_name);
			if (is_pipex)
				exit(126);
			return (-1);
		}
		if (access(file_name, F_OK) == 0 && access(file_name, W_OK) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file_name, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			shell->exit_status = 1;
			free(file_name);
			if (is_pipex)
				exit(1);
			return (-1);
		}
		file_out = open(file_name, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (file_out == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("failed to open file\n", 2);
			shell->exit_status = 1;
			free(file_name);
			if (is_pipex)
				exit(1);
			return (-1);
		}
	}
	else
	{
		if (is_directory(file_name))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file_name, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			shell->exit_status = 126;
			free(file_name);
			if (is_pipex)
				exit(126);
			return (-1);
		}
		if (access(file_name, F_OK) == 0 && access(file_name, W_OK) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file_name, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			shell->exit_status = 1;
			free(file_name);
			if (is_pipex)
				exit(1);
			return (-1);
		}
		file_out = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (file_out == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("failed to open file\n", 2);
			shell->exit_status = 1;
			free(file_name);
			if (is_pipex)
				exit(1);
			return (-1);
		}
	}
	free(file_name);
	return (file_out);
}

static int	redirect_input(t_shell *shell, char **args, int i, int is_pipex)
{
	int		file_in;
	int		is_heredoc;
	char	*file_name;
	char	*heredoc_name;
	int		arg_nr;

	arg_nr = 0;
	while (args[arg_nr])
		arg_nr++;
	arg_nr = shell->arg_count - arg_nr;
	is_heredoc = 0;
	if (args[0][i + 1] == args[0][i])
	{
		is_heredoc = 1;
		i++;
	}
	file_name = get_file_name(shell, args, i);
	if (file_name == NULL)
		return (-1);
	if (is_heredoc)
	{
		heredoc_name = ft_strjoinfree2(".heredoc", ft_itoa(arg_nr));
		file_in = open(heredoc_name, O_RDONLY);
		if (file_in == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("failed to open heredoc file\n", 2);
			shell->exit_status = 1;
			free(file_name);
			free(heredoc_name);
			if (is_pipex)
				exit(1);
			return (-1);
		}
		free(heredoc_name);
	}
	else
	{
		if (is_directory(file_name))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file_name, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			shell->exit_status = 126;
			free(file_name);
			if (is_pipex)
				exit(126);
			return (-1);
		}
		if (access(file_name, F_OK) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file_name, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			shell->exit_status = 1;
			free(file_name);
			if (is_pipex)
				exit(1);
			return (-1);
		}
		if (access(file_name, R_OK) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file_name, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			shell->exit_status = 1;
			free(file_name);
			if (is_pipex)
				exit(1);
			return (-1);
		}
		file_in = open(file_name, O_RDONLY);
		if (file_in == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("failed to open file\n", 2);
			shell->exit_status = 1;
			free(file_name);
			if (is_pipex)
				exit(1);
			return (-1);
		}
	}
	free(file_name);
	return (file_in);
}

static int	redirects_main_handler(t_shell *shell, char **args, int *file_in, int *file_out, int is_pipex)
{
	int	i;

	i = 0;
	while ((*args)[i])
	{
		if (is_c_not_in_quotes(*args, i, '<'))
		{
			if (*file_in != -1)
				close(*file_in);
			*file_in = redirect_input(shell, args, i, is_pipex);
			if ((*args)[i + 1] == (*args)[i])
				i++;
			if (*file_in == -1)
				return (-1);
		}
		if (is_c_not_in_quotes(*args, i, '>'))
		{
			if (*file_out != -1)
				close(*file_out);
			*file_out = redirect_output(shell, args, i, is_pipex);
			if ((*args)[i + 1] == (*args)[i])
				i++;
			if (*file_out == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}

char	*remove_redirects(char **new_input, char **args)
{
	int		j;
	int		k;
	char	*prev;
	char	*next;

	j = 0;
	while ((*args)[j] && !is_c_not_in_quotes(*args, j, '|'))
	{
		if (is_c_not_in_quotes(*args, j, '<') || is_c_not_in_quotes \
		(*args, j, '>'))
		{
			prev = ft_substr(*args, 0, j);
			k = j;
			if ((*args)[j + 1] == (*args)[j])
				k++;
			k++;
			while ((*args)[k] && is_c_not_in_quotes(*args, k, ' '))
				k++;
			while ((*args)[k] && !is_c_not_in_quotes(*args, k, ' ') \
			&& !is_c_not_in_quotes(*args, k, '|') && !is_c_not_in_quotes \
			(*args, k, '<') && !is_c_not_in_quotes(*args, k, '>'))
				k++;
			next = ft_strdup(*args + k);
			*new_input = ft_strjoin(prev, next);
			free(prev);
			free(next);
			return (*new_input);
		}
		j++;
	}
	return (NULL);
}

int	redirects_handler(t_shell *shell, int i, int **fds, char **args, int is_pipex)
{
	int		file_in;
	int		file_out;
	char	*new_input;

	file_in = -1;
	file_out = -1;
	new_input = NULL;
	redirects_main_handler(shell, args, &file_in, &file_out, is_pipex);
	while (remove_redirects(&new_input, args))
	{
		free(*args);
		*args = new_input;
	}
	if (i == 0 && i != shell->arg_count - 1)
	{
		if (file_out == -1)
			dup2(fds[i][1], STDOUT_FILENO);
		else
		{
			dup2(file_out, STDOUT_FILENO);
			close(fds[i][1]);
		}
		close(fds[i][0]);
		if (file_in != -1)
			dup2(file_in, STDIN_FILENO);
	}
	else if (i == shell->arg_count - 1 && i != 0)
	{
		if (file_in == -1)
			dup2(fds[i - 1][0], STDIN_FILENO);
		else
		{
			dup2(file_in, STDIN_FILENO);
			close(fds[i - 1][0]);
		}
		close(fds[i - 1][1]);
		if (file_out != -1)
			dup2(file_out, STDOUT_FILENO);
	}
	else if (i != 0 && i != shell->arg_count - 1)
	{
		if (file_in == -1)
			dup2(fds[i - 1][0], STDIN_FILENO);
		else
		{
			dup2(file_in, STDIN_FILENO);
			close(fds[i - 1][0]);
		}
		if (file_out == -1)
			dup2(fds[i][1], STDOUT_FILENO);
		else
		{
			dup2(file_out, STDOUT_FILENO);
			close(fds[i][1]);
		}
		close(fds[i - 1][1]);
		close(fds[i][0]);
	}
	else if (i == 0 && i == shell->arg_count - 1)
	{
		if (file_out != -1)
			dup2(file_out, STDOUT_FILENO);
		if (file_in != -1)
			dup2(file_in, STDIN_FILENO);
	}
	return (0);
}

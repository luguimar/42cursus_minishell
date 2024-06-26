/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 10:04:45 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/26 23:30:13 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	redirects_input(t_shell *shell, int *i, int **fds, int in, int is_pipex)
{
	int		j;
	int		k;
	char	*file;
	int		fd;

	k = 0;
	j = 0;
	while (j < *i)
	{
		if (is_c_not_in_quotes(shell->input, j, '|'))
			k++;
		j++;
	}
	(*i)++;
	while (shell->input[*i] && is_c_not_in_quotes(shell->input, *i, ' '))
		(*i)++;
	j = *i;
	while (shell->input[j] && !is_c_not_in_quotes(shell->input, j, ' ') \
	&& !is_c_not_in_quotes(shell->input, j, '|') && !is_c_not_in_quotes(shell \
	->input, j, '<') && !is_c_not_in_quotes(shell->input, j, '>'))
		j++;
	file = ft_substr(shell->input, *i, j - *i);
	file = ft_remove_quotes(file);
	if (in)
	{
		if (k != 0)
			close(fds[k - 1][0]);
		if (access(file, F_OK) != 0 || access(file, R_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			free(file);
			free_everything(shell);
			k = -1;
			while (++k < shell->arg_count - 1)
				free(fds[k]);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
		fd = open(file, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			free(file);
			free_everything(shell);
			k = -1;
			while (++k < shell->arg_count - 1)
				free(fds[k]);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
		free(file);
	}
	else
	{
		if (access(file, F_OK) != 0 || access(file, R_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			free(file);
			free_everything(shell);
			k = -1;
			while (++k < shell->arg_count - 1)
				free(fds[k]);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
		fd = open(file, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			free(file);
			free_everything(shell);
			k = -1;
			while (++k < shell->arg_count - 1)
				free(fds[k]);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
		free(file);
		close(fd);
		fd = -1;
	}
	free(file);
	*i = j;
	return (fd);
}

static int	redirects_heredoc(t_shell *shell, int *i, int **fds, int out, int is_pipex)
{
	int		j;
	int		k;
	char	*heredoc_name;
	int		fd;

	j = 0;
	k = 0;
	while (j < *i)
	{
		if (is_c_not_in_quotes(shell->input, j, '|'))
			k++;
		j++;
	}
	*i += 2;
	while (shell->input[*i] && is_c_not_in_quotes(shell->input, *i, ' '))
		(*i)++;
	j = *i;
	while (shell->input[j] && !is_c_not_in_quotes(shell->input, j, ' ') \
	&& !is_c_not_in_quotes(shell->input, j, '|') && !is_c_not_in_quotes(shell \
	->input, j, '<') && !is_c_not_in_quotes(shell->input, j, '>'))
		j++;
	if (out)
	{
		if (k != 0)
			close(fds[k - 1][0]);
		heredoc_name = ft_strjoinfree2(".heredoc", ft_itoa(k));
		if (heredoc_name == NULL)
			return (-1);
		if (access(heredoc_name, F_OK) != 0 || access(heredoc_name, R_OK) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(heredoc_name);
			free(heredoc_name);
			free_everything(shell);
			k = -1;
			while (++k < shell->arg_count - 1)
				free(fds[k]);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
		fd = open(heredoc_name, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(heredoc_name);
			free(heredoc_name);
			free_everything(shell);
			k = -1;
			while (++k < shell->arg_count - 1)
				free(fds[k]);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
		free(heredoc_name);
	}
	else
		fd = -1;
	*i = j;
	return (fd);
}

static int	redirects_in_extra(t_shell *shell, int last_input, int **fds, int is_pipex)
{
	int	i;

	i = last_input;
	while (i > 0 && !is_c_not_in_quotes(shell->input, i, '|'))
		i--;
	while (shell->input[i] && i != last_input)
	{
		if (is_c_not_in_quotes(shell->input, i, '<'))
		{
			if (shell->input[i + 1] == '<')
				redirects_heredoc(shell, &i, fds, 0, is_pipex);
			else
				redirects_input(shell, &i, fds, 0, is_pipex);
		}
		i++;
	}
	if (shell->input[i + 1] == '<')
		return (redirects_heredoc(shell, &i, fds, 1, is_pipex));
	else
		return (redirects_input(shell, &i, fds, 1, is_pipex));
}

static int	redirects_in_handler(t_shell *shell, int i, int **fds, int is_pipex)
{
	int		last_input;
	int		j;

	last_input = 0;
	j = -1;
	while (shell->input[++j])
	{
		if (last_input == i)
			break ;
		if (is_c_not_in_quotes(shell->input, j, '|'))
			last_input++;
	}
	last_input = -1;
	while (shell->input[j] && !is_c_not_in_quotes(shell->input, j, '|'))
	{
		if (is_c_not_in_quotes(shell->input, j, '<'))
		{
			last_input = j;
			if (shell->input[j + 1] == '<')
				j++;
		}
		j++;
	}
	if (last_input == -1)
		return (-1);
	else
		return (redirects_in_extra(shell, last_input, fds, is_pipex));
}

static int	redirects_output(t_shell *shell, int *i, int **fds, int out, int is_pipex)
{
	int		j;
	int		k;
	int		l;
	char	*file;
	int		fd;

	k = 0;
	j = 0;
	while (j < *i)
	{
		if (is_c_not_in_quotes(shell->input, j, '|'))
			k++;
		j++;
	}
	(*i)++;
	while (shell->input[*i] && is_c_not_in_quotes(shell->input, *i, ' '))
		(*i)++;
	j = *i;
	while (shell->input[j] && !is_c_not_in_quotes(shell->input, j, ' ') \
	&& !is_c_not_in_quotes(shell->input, j, '|') && !is_c_not_in_quotes(shell \
	->input, j, '<') && !is_c_not_in_quotes(shell->input, j, '>'))
		j++;
	file = ft_substr(shell->input, *i, j - *i);
	file = ft_remove_quotes(file);
	if (out)
	{
		if (k != shell->arg_count - 1)
			close(fds[k][1]);
		if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			free(file);
			free_everything(shell);
			l = -1;
			while (++l < shell->arg_count - 1)
				free(fds[l]);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			free(file);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
	}
	else
	{
		if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			free(file);
			free_everything(shell);
			l = -1;
			while (++l < shell->arg_count - 1)
				free(fds[l]);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			free(file);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
		close(fd);
		fd = -1;
	}
	free(file);
	*i = j;
	return (fd);
}

static int	redirects_append(t_shell *shell, int *i, int **fds, int out, int is_pipex)
{
	int		j;
	int		k;
	int		l;
	char	*file;
	int		fd;

	k = 0;
	j = 0;
	while (j < *i)
	{
		if (is_c_not_in_quotes(shell->input, j, '|'))
			k++;
		j++;
	}
	(*i) += 2;
	while (shell->input[*i] && is_c_not_in_quotes(shell->input, *i, ' '))
		(*i)++;
	j = *i;
	while (shell->input[j] && !is_c_not_in_quotes(shell->input, j, ' ') \
	&& !is_c_not_in_quotes(shell->input, j, '|') && !is_c_not_in_quotes(shell \
	->input, j, '<') && !is_c_not_in_quotes(shell->input, j, '>'))
		j++;
	file = ft_substr(shell->input, *i, j - *i);
	file = ft_remove_quotes(file);
	if (out)
	{
		if (k != shell->arg_count - 1)
			close(fds[k][1]);
		if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			free(file);
			free_everything(shell);
			l = -1;
			while (++l < shell->arg_count - 1)
				free(fds[l]);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			free(file);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
	}
	else
	{
		if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			free(file);
			free_everything(shell);
			l = -1;
			while (++l < shell->arg_count - 1)
				free(fds[l]);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(file);
			free(file);
			if (is_pipex)
				exit(1);
			else
			{
				shell->exit_status = 1;
				return (-1);
			}
		}
		close(fd);
		fd = -1;
	}
	free(file);
	*i = j;
	return (fd);
}

static int	redirects_out_handler_extra(t_shell *shell, int last_output, int **fds, int is_pipex)
{
	int	i;

	i = last_output;
	while (i > 0 && !is_c_not_in_quotes(shell->input, i, '|'))
		i--;
	while (shell->input[i] && i != last_output)
	{
		if (is_c_not_in_quotes(shell->input, i, '>'))
		{
			if (shell->input[i + 1] == '>')
				redirects_append(shell, &i, fds, 0, is_pipex);
			else
				redirects_output(shell, &i, fds, 0, is_pipex);
		}
		i++;
	}
	if (shell->input[i + 1] == '>')
		return (redirects_append(shell, &i, fds, 1, is_pipex));
	else
		return (redirects_output(shell, &i, fds, 1, is_pipex));
}

static int	redirects_out_handler(t_shell *shell, int i, int **fds, int is_pipex)
{
	int		j;
	int		k;
	int		last_output;

	j = -1;
	k = 0;
	last_output = 0;
	while (shell->input[++j])
	{
		if (k == i)
			break ;
		if (is_c_not_in_quotes(shell->input, j, '|'))
			k++;
	}
	last_output = -1;
	while (shell->input[j] && !is_c_not_in_quotes(shell->input, j, '|'))
	{
		if (is_c_not_in_quotes(shell->input, j, '>'))
		{
			last_output = j;
			if (shell->input[j + 1] == '>')
				j++;
		}
		j++;
	}
	if (last_output == -1)
		return (-1);
	else
		return (redirects_out_handler_extra(shell, last_output, fds, is_pipex));
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

	new_input = NULL;
	file_in = redirects_in_handler(shell, i, fds, is_pipex);
	file_out = redirects_out_handler(shell, i, fds, is_pipex);
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

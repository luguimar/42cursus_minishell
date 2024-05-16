/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 10:04:45 by luguimar          #+#    #+#             */
/*   Updated: 2024/05/16 21:18:20 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	redirects_input(t_shell *shell, int *i, int **fds, int in)
{
	int		j;
	int		k;
	char	*file;
	int		fd;

	k = 0;
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
		fd = open(file, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			free(file);
			return (-1);
		}
	}
	else
	{
		fd = open(file, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			free(file);
			return (-1);
		}
		close(fd);
	}
	free(file);
	*i = j;
	return (fd);
}

static int	redirects_heredoc(t_shell *shell, int *i, int **fds, int out)
{
	char	*line;
	int		j;
	int		k;
	char	*terminator;
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
	terminator = ft_substr(shell->input, *i, j - *i);
	terminator = ft_remove_quotes(terminator);
	terminator = ft_strjoinfree(terminator, "\n");
	if (out)
	{
		if (k != 0)
			close(fds[k - 1][0]);
		fd = open(".here_doc", O_RDWR | O_TRUNC | O_CREAT, 0664);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(".here_doc", 2);
			ft_putstr_fd(": failed to open\n", 2);
			free(terminator);
			return (-1);
		}
		ft_putchar_fd('>', 1);
		line = get_next_line(0);
		if (line && ft_strcmp(line, terminator) != 0)
			write(fd, line, ft_strlen(line));
		while (line && ft_strcmp(line, terminator) != 0)
		{
			free(line);
			ft_putchar_fd('>', 1);
			line = get_next_line(0);
			if (line && ft_strcmp(line, terminator) != 0)
				write(fd, line, ft_strlen(line));
		}
		free(line);
		free(terminator);
		close(fd);
		fd = open(".here_doc", O_RDONLY);
	}
	else
	{
		fd = -1;
		line = get_next_line(0);
		while (line && ft_strcmp(line, terminator) != 0)
		{
			free(line);
			line = get_next_line(0);
		}
		free(line);
		free(terminator);
	}
	*i = j;
	return (fd);
}

static int	redirects_in_extra(t_shell *shell, int last_input, int **fds)
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
				redirects_heredoc(shell, &i, fds, 0);
			else
				redirects_input(shell, &i, fds, 0);
		}
		i++;
	}
	if (shell->input[i + 1] == '<')
		return (redirects_heredoc(shell, &i, fds, 1));
	else
		return (redirects_input(shell, &i, fds, 1));
}

static int	redirects_in_handler(t_shell *shell, int i, int **fds)
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
		return (redirects_in_extra(shell, last_input, fds));
}

static int	redirects_output(t_shell *shell, int *i, int **fds, int out)
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
	if (out)
	{
		if (k != shell->arg_count - 1)
			close(fds[k][1]);
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			free(file);
			return (-1);
		}
	}
	else
	{
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			free(file);
			return (-1);
		}
		close(fd);
	}
	free(file);
	*i = j;
	return (fd);
}

static int	redirects_append(t_shell *shell, int *i, int **fds, int out)
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
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			free(file);
			return (-1);
		}
	}
	else
	{
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(file, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			free(file);
			return (-1);
		}
		close(fd);
	}
	free(file);
	*i = j;
	return (fd);
}

static int	redirects_out_handler_extra(t_shell *shell, int last_output, int **fds)
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
				redirects_append(shell, &i, fds, 0);
			else
				redirects_output(shell, &i, fds, 0);
		}
		i++;
	}
	if (shell->input[i + 1] == '>')
		return (redirects_append(shell, &i, fds, 1));
	else
		return (redirects_output(shell, &i, fds, 1));
}

static int	redirects_out_handler(t_shell *shell, int i, int **fds)
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
		return (redirects_out_handler_extra(shell, last_output, fds));
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

int	redirects_handler(t_shell *shell, int i, int **fds, char **args)
{
	int		file_in;
	int		file_out;
	char	*new_input;

	new_input = NULL;
	file_in = redirects_in_handler(shell, i, fds);
	file_out = redirects_out_handler(shell, i, fds);
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
			close(fds[i][1]);
			dup2(file_out, STDOUT_FILENO);
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
			close(fds[i - 1][0]);
			dup2(file_in, STDIN_FILENO);
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
			close(fds[i - 1][0]);
			dup2(file_in, STDIN_FILENO);
		}
		if (file_out == -1)
			dup2(fds[i][1], STDOUT_FILENO);
		else
		{
			close(fds[i][1]);
			dup2(file_out, STDOUT_FILENO);
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

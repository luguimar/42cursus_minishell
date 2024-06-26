/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 04:01:49 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/26 20:17:34 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	heredocs(char *arg, int count, t_shell *shell)
{
	int		i;
	int		j;
	int		k;
	int		fd;
	char	*line;
	char	*eof;
	char	*heredoc_name;

	i = -1;
	shell->heredocs[count] = 0;
	while (arg[++i])
		if (arg[i] == '<' && arg[i + 1] == '<')
			shell->heredocs[count]++;
	if (shell->heredocs[count] == 0)
		return (0);
	i = 0;
	eof = malloc(1);
	j = 0;
	while (++i < shell->heredocs[count])
	{
		j = i - 1;
		while (arg[++j])
		{
			if (arg[j] == '<' && arg[j + 1] == '<')
			{
				j++;
				while (arg[++j] && (arg[j] == ' ' || arg[j] == '\t'))
					;
				k = j;
				while (arg[k] && !is_c_not_in_quotes(arg, k, ' ') && \
				!is_c_not_in_quotes(arg, k, '<') && \
				!is_c_not_in_quotes(arg, k, '>'))
					k++;
				free(eof);
				eof = ft_substr(arg, j, k - j);
				eof = ft_remove_quotes(eof);
				eof = ft_strjoinfree(eof, "\n");
				break ;
			}
		}
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		while (line && ft_strcmp(line, eof))
		{
			free(line);
			ft_putstr_fd("> ", 1);
			line = get_next_line(0);
		}
		free(line);
	}
	free(eof);
	while (arg[j] && !(arg[j] == '<' && arg[j + 1] == '<'))
		j++;
	j++;
	while (arg[++j] && (arg[j] == ' '))
		;
	k = j;
	while (arg[k] && !is_c_not_in_quotes(arg, k, ' ') && \
	!is_c_not_in_quotes(arg, k, '<') && !is_c_not_in_quotes(arg, k, '>'))
		k++;
	eof = ft_substr(arg, j, k - j);
	eof = ft_remove_quotes(eof);
	eof = ft_strjoinfree(eof, "\n");
	heredoc_name = ft_strjoinfree2(".heredoc", ft_itoa(count));
	fd = open(heredoc_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	ft_putstr_fd("> ", 1);
	line = get_next_line(0);
	while (line && ft_strcmp(line, eof))
	{
		expand(&line, shell, -1, 0);
		write(fd, line, ft_strlen(line));
		free(line);
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
	}
	free(line);
	free(eof);
	free(heredoc_name);
	close(fd);
	return (shell->heredocs[count]);
}

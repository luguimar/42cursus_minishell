/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:24:19 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/12 20:40:55 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>

void	dup2stdout(int *pipefd);
void	dup2stdin(int *pipefd);
void	dup2redirect(int *fd, char **argv, char **envp, int i);
void	redirect_files(int i, char *argv[], char **envp);
void	heredoc(char *limiter);
char	*get_right_path(char **cmd, char **envp, char *right_path);
char	**last_one(char **argv, char **path, char **envp, int i);
void	check_error(int status, char *message, char **args, char *path);
int		pipex(int argc, char **argv, char **envp);

#endif

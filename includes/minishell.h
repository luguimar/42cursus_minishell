/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:24:19 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/17 14:24:18 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>

typedef struct s_env
{
	char	*key;
	char	*value;
	char	*full;
}				t_env;

typedef struct s_shell
{
	char	*input;
	char	**env_array;
	t_list	*env;
}				t_shell;

//builtins
int		exec_builtin(char **args, t_shell *shell);
//cd
int		ft_cd_case_double_dash(t_shell *shell, char ***args);
char	*ft_strremoveprev(char *str, char *old, char *new);
char	*ft_strreplace(char *str, char *old, char *new);
char	*ft_cd_check_for_dots(char **args, t_shell *shell);
int		ft_cd_normal(char **args, t_shell *shell);
int		ft_cd_case_dotdot(t_shell *shell, char ***args);
int		ft_cd_aux_extra(t_shell *shell, char ***args);
int		ft_cd_case_slash(t_shell *shell, char ***args);
int		ft_cd_case_tild(t_shell *shell, char ***args);
int		ft_cd_aux(char **args, t_shell *shell);
//unset
void	delete_env_node(t_list **env, char *key);
//env
int		ft_env(char **args, t_shell *shell);

//envparser

int		env_to_list(t_shell *shell, char **envp);
t_env	*envnew(char *key, char *value, char *env_i);
char	**env_to_array(t_list *env);
void	add_env(t_shell *shell, char *key, char *value);
void	change_value(t_list *env, char *key, char *value);
char	*get_env_value(t_list *env, char *key);
void	free_everything(t_shell *shell);

//pipes

void	dup2stdout(int *pipefd);
void	dup2stdin(int *pipefd);
void	dup2redirect(int *fd, char **argv, t_shell *shell, int i);
void	redirect_files(int i, char *argv[], t_shell *shell);
void	heredoc(char *limiter);
char	*get_right_path(char **cmd, char **envp, char *right_path);
char	**last_one(char **argv, char **path, char **envp, int i);
void	check_error(int status, char *message, char **args, char *path);
int		pipex(int argc, char **argv, t_shell *shell);
int		get_right_path_aux(char **cmd, char **path, int i, char **right_path);

#endif

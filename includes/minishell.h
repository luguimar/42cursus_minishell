/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:24:19 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/11 20:47:23 by luguimar         ###   ########.fr       */
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
	char	*export_version;
	int		is_just_exported;

}				t_env;

typedef struct s_shell
{
	int		proccess_status;
	int		exit_status;
	int		*pids;
	int		arg_count;
	char	*input;
	char	**env_array;
	t_list	*env;
}				t_shell;

//builtins
int		exec_builtin(char **args, t_shell *shell, int ispipex);
int		ft_echo(char **args);
int		ft_exit(char **args, t_shell *shell, char **old_args);
//cd
int		ft_cd_case_double_dash(t_shell *shell, char ***args);
char	*ft_strremoveprev(char *str, char *old, char *new);
char	*ft_strreplace(char *str, char *old, char *new);
char	*ft_cd_check_for_dots(char *args);
int		ft_cd_normal(char **args, t_shell *shell);
int		ft_cd_case_dotdot(t_shell *shell, char ***args);
int		ft_cd_aux_extra(t_shell *shell, char ***args);
int		ft_cd_case_slash(t_shell *shell, char ***args);
int		ft_cd_case_tild(t_shell *shell, char ***args);
int		ft_cd_aux(char **args, t_shell *shell);
int		ft_cd_case_dash(t_shell *shell, char **args);
//unset
void	delete_env_node(t_list **env, char *key);
void	free_env(void *content);
//env
int		ft_env(char **args, t_shell *shell);
int		ft_export(char **args, t_shell *shell);

//parser

int		env_to_list(t_shell *shell, char **envp);
t_env	*envnew(char *key, char *value, char *env_i, int is_just_exported);
char	**env_to_array(t_list *env);
void	add_env(t_shell *shell, char *key, char *value);
int		change_value(t_list *env, char *key, char *value);
char	*get_env_value(t_list *env, char *key);
void	free_everything(t_shell *shell);
void	expand(char **input, t_shell *shell, int i, int s);

//pipes

void	dup2pipe(int **fds, int i, t_shell *shell, char **args);
void	dup2redirect(int *fd, char **argv, t_shell *shell, int i);
void	heredoc(char *limiter);
void	redirect_files_aux(int cid, int i, t_shell *shell, int ***fds);
char	*get_right_path(char **cmd, char **envp, char *right_path);
char	**last_one(char **argv, char **path, char **envp, int i);
void	check_error(int status, char *message, char **args, char *path);
int		pipex(int argc, char **argv, t_shell *shell);
int		get_right_path_aux(char **cmd, char **path, int i, char **right_path);
int		execve_error(char *path, char **args, t_shell *shell, int isparent);
char	*ft_getdirs(char *cmd);
int		get_right_path_aux2(char **cmd, char **right_path);

//signals

void	main_handler(int signum);
void	sigint_parent(int signum);

//redirects

int		redirects_handler(t_shell *shell, int i, int **fds, char **args);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 02:27:37 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/19 02:29:25 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execve_error(char *path, char **args, t_shell *shell, int isparent)
{
	if (isparent)
		wait(NULL);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	perror("minishell");
	free_everything(shell);
	free(path);
	free_array_of_strings(args);
	return (1);
}

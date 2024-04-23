/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:58:44 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/20 02:35:33 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_env(char **args, t_shell *shell)
{
	t_list	*tmp;

	if (ft_matrixlen((void **)args) > 1)
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		free_array_of_strings(args);
		return (1);
	}
	tmp = shell->env;
	while (tmp)
	{
		ft_putstr_fd(((t_env *)tmp->content)->full, 1);
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
	free_array_of_strings(args);
	return (1);
}

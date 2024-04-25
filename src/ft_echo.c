/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 19:06:03 by jduraes-          #+#    #+#             */
/*   Updated: 2024/04/25 20:54:56 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_echo(char **args)
{
	int	i;
	int	n;

	i = 1;
	n = 0;
	if (!args[1])
		return (ft_putchar_fd('\n', 1));
	if (ft_strncmp(args[1], "-n", 2) == 0 && args[1][2] == '\0')
	{
		n = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i++ + 1])
			ft_putchar_fd(' ', 1);
	}
	if (!n)
		ft_putchar_fd('\n', 1);
	/*(void)args;
	ft_putstr_fd("echo\n", 1);
	ft_putchar_fd('\n', 1);*/
	return (1);
}

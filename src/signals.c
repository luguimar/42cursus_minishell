/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 20:01:48 by jduraes-          #+#    #+#             */
/*   Updated: 2024/04/23 20:25:26 by jduraes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	sighandler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else
		return ;
}

static void	cathandler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	else
		return ;
}

void	sigset(int a)
{
	if (a == 1)
	{
		signal(SIGINT, sighandler);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	if (a == 2)
		signal(SIGINT, cathandler);
}

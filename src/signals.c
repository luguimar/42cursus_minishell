/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduraes- <jduraes-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 20:01:48 by jduraes-          #+#    #+#             */
/*   Updated: 2024/06/04 20:40:01 by jduraes-         ###   ########.fr       */
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

static void	dumphandler(int sig)
{
	t_shell	*shell;

	shell = getshell(0);
	if (sig == SIGQUIT)
	{
		kill(shell->pids[shell->arg_count - 1], SIGQUIT);
	}
	else
		return ;
}

static void	cdumphandler(int sig)
{
	//t_shell	*shell;

	//shell = getshell(0);
	if (sig == SIGQUIT)
	{
		write(1, "Quit (core dumped)\n", 19);
		//kill(shell->pids[shell->arg_count - 1], SIGQUIT);
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
	if (a == 3)
		signal(SIGQUIT, cdumphandler);
	if (a == 4)
		signal(SIGINT, dumphandler);
}

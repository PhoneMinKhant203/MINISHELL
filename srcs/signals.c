/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 14:02:59 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/01 16:41:57 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

#define _POSIX_C_SOURCE 200809L

volatile sig_atomic_t	g_signal = 0;

void	handle_sigint(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/*
rl_on_new_line  -> readline moved to a new line
rl_replace_line -> erase current input
rl_redisplay    -> recall new prompt

sa.sa_handler = handle_sigint  -> call handler() when SIGINT happens
sigemptyset(&sa.sa_mask)       -> handling SIGINT, clear all signals
sa.sa_flags = SA_RESTART       -> 
sigaction()
signal()
*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:57:01 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/04 15:20:46 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <stdbool.h>
# include <errno.h>
# include <sys/wait.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

extern volatile sig_atomic_t	g_signal;

typedef struct s_cmd
{
    char			**args;
    char			*infile;
	char			*outfile;
	int				append;
    struct s_cmd	*next;
}	t_cmd;

typedef enum e_tktype
{
	T_WORD,
	T_PIPE,
	T_IN,
	T_OUT,
	T_APPEND,
	T_HEREDOC
}	t_tktype;

typedef struct s_token
{
	char			*value;
	t_tktype		type;
	struct s_token	*next;
}	t_token;

//Utils
void	free2p(char **s);
void	free1p(char **s);
void	free_cmds(t_cmd *cmd);
void	free_tokens(t_token *tok);

//Main
void	setup_signals(void);

//Lexer
int		skip_spaces(char *s, int *i);
int		is_operator(char c);
char	*get_word(char *s, int *i);
t_token	*new_token(char *val, t_tktype type);
t_token	*get_token(char *s, int *i);
t_token	*tokenize(char *line);

//Parser
t_cmd	*new_cmd(void);
int		count_args(t_token *tok);
void	fill_args(t_cmd *cmd, t_token *tok);
void	handle_redir(t_cmd *cmd, t_token *tok);
t_cmd	*parse_one_cmd(t_token *tok);
t_token	*skip_to_pipe(t_token *tok);
t_cmd	*parse(t_token *tok);

int		exe_cmd(char **args, char **envp);

#endif
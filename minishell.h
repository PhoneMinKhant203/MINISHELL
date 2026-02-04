/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:57:01 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/04 18:38:07 by wintoo           ###   ########.fr       */
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
	char			*heredoc;
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

typedef struct	s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env;
	int		last_status;
}	t_shell;

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

//expand
void	expand_args(char **args, t_shell *sh);
char	*expand_str(char *s, t_shell *sh);
char	*handle_single_quote(char *res, char *s, int *i);
char	*handle_double_quote(char *res, char *s, int *i, t_shell *sh);
char	*handle_dollar(char *res, char *s, int *i, t_shell *sh);
char	*append_char(char *s, char c);
char	*append_str(char *s1, char *s2);
void	expand_redirs(t_cmd *cmd, t_shell *sh);
void	expand_cmds(t_cmd *cmds, t_shell *sh);

//Built in
int		is_builtin(char *cmd);
int		exec_builtin(char **args, t_env **env_copy);
t_env	*init_env(char **envp);
int		mini_cd(char **args);
int		mini_echo(char **args);
int		mini_pwd(void);
int		mini_env(t_env *env);
void	mini_exit(char **args);
t_env	*find_env_node(t_env *env, char *key);
void	add_or_update_env(t_env **env, char *key, char *value);
int		mini_export(char **args, t_env **env);
int		mini_unset(char **args, t_env **env);
int		is_valid_var_name(char *str);
void	sort_env_list(t_env *head);
void	print_sorted_env(t_env *env);
t_env	*copy_env_list(t_env *env);
void	free_env_list(t_env *env);

int		exe_cmd(char **args, char **envp);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:57:01 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/07 18:06:31 by wintoo           ###   ########.fr       */
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
# include <sys/stat.h>
# include <sys/wait.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

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

typedef struct s_env
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

extern volatile sig_atomic_t	g_signal;

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

//Utils
void	free2p(char **s);
void	free1p(char **s);
void	free_cmds(t_cmd *cmd);
void	free_tokens(t_token *tok);
void	free_env(t_env *env);
int		print_err(char *cmd, char mode);

//Built in
int		is_builtin(char *cmd);
int		exe_builtin(char **args, t_shell *sh);
t_env	*init_env(char **envp);
int		mini_cd(char **args, t_env **env);
int		mini_echo(char **args);
int		mini_pwd(void);
int		mini_env(t_env *env);
void	mini_exit(char **args, t_shell *sh);
t_env	*find_env_node(t_env *env, char *key);
void	add_or_update_env(t_env **env, char *key, char *value);
int		mini_export(char **args, t_env **env);
int		mini_unset(char **args, t_env **env);
int		is_valid_var_name(char *str);
void	sort_env_list(t_env *head);
void	print_sorted_env(t_env *env);
t_env	*copy_env_list(t_env *env);

//Expand
char	*env_get(t_env *env, char *key);
char	*append_str(char *s, char *to_append);
char	*append_char(char *res, char c);
void	handle_dollar(char **res, char *s, int *i, t_shell *sh);
void	handle_quote(char **res, char *s, int *i, t_shell *sh);
char	*expand_str(char *s, t_shell *sh);
void	expand_cmds(t_cmd *cmds, t_shell *sh);

//Redirection
int		setup_redirection(t_cmd *cmd);
void	handle_heredoc(char *delimiter);

//Signal
void	setup_signals(void);

//Free
void	free2p(char **s);
void	free_tokens(t_token *tok);
void	free_cmds(t_cmd *cmd);
void	free1p(char **s);

//CMD_Execution
void	start_executor(t_cmd *cmds, t_shell *sh);
void	child_exec_binary(t_cmd *cmd, t_shell *sh);
void	wait_all_children(t_shell *sh);

//Execucator
char	**env_to_array(t_env *env);
char	*find_path(char *cmd, t_env *env_list);
int		exe_cmd(t_cmd *cmd, t_env *env_list);
int		execute_cmds(t_cmd *cmds, t_shell *sh);

#endif
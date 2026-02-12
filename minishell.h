/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:57:01 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/12 16:57:34 by wintoo           ###   ########.fr       */
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

typedef enum e_tktype
{
	T_WORD,
	T_PIPE,
	T_AND,
	T_OR,
	T_IN,
	T_OUT,
	T_APPEND,
	T_HEREDOC,
	T_BAD
}	t_tktype;

typedef struct s_redir
{
	t_tktype			type;
	char				*target;
	struct s_redir		*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	char			*infile;
	char			*outfile;
	int				append;
	char			*heredoc;
	int				fd_in;
	int				fd_out;
	struct s_cmd	*next;
}	t_cmd;

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

typedef enum e_ntype
{
	N_PIPELINE,
	N_AND,
	N_OR
}	t_ntype;

typedef struct s_node
{
	t_ntype			type;
	struct s_node	*left;
	struct s_node	*right;
	t_cmd			*pipeline;
}	t_node;

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

//Redirection
t_redir	*new_redir(t_tktype type, const char *target);
void	redir_add_back(t_cmd *cmd, t_redir *node);
int		setup_redirection(t_cmd *cmd);
void	handle_heredoc(char *delimiter);

//Utils
void	free2p(char **s);
void	free1p(char **s);
void	free_redirs(t_redir *r);
void	free_cmds(t_cmd *cmd);
void	free_tokens(t_token *tok);
void	free_env(t_env *env);
void	print_err(char *arg, char *cmd, char *msg);
void	exe_error(char *arg, char *path);

//Expand
t_env	*new_env_node(char *key, char *value);
char	*env_get(t_env *env, char *key);
char	*append_str(char *s, char *to_append);
char	*append_char(char *res, char c);
void	handle_dollar(char **res, char *s, int *i, t_shell *sh);
void	handle_quote(char **res, char *s, int *i, t_shell *sh);
char	*expand_str(char *s, t_shell *sh);
void	expand_cmds(t_cmd *cmds, t_shell *sh);

//Built in
int		is_builtin(char **args);
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

//Signal
void	setup_signals(void);

//Free
void	free2p(char **s);
void	free_tokens(t_token *tok);
void	free_cmds(t_cmd *cmd);
void	free1p(char **s);

//CMD_Execution
void	start_executor(t_cmd *cmds, t_shell *sh);
void	child_exec_binary(t_cmd *cmd, t_shell *sh, int i);
void	wait_all_children(t_shell *sh, pid_t last_pid);

//Execucator
char	**env_to_array(t_env *env);
char	*find_path(char *cmd, t_env *env_list);
int		exe_cmd(t_cmd *cmd, t_env *env_list);
int		execute_cmds(t_cmd *cmds, t_shell *sh);

// Wildcard
int		contains_wildcard(const char *s);
int		match_pattern(const char *pat, const char *name);
char	**expand_wildcards_argv(char **argv, int *changed);
char	*expand_wildcard_redir(const char *pattern, int *ambiguous);
void	unmask_wildcards(char *s);
char	mask_wildcard_char(char c);

int		validate_syntax(t_token *tk, t_shell *sh);
/* Parser (bonus && ||) */
t_node	*parse_ast(t_token *tok);
void	free_ast(t_node *node);

/* Executor (bonus && ||) */
int		execute_ast(t_node *node, t_shell *sh);
void	expand_ast(t_node *node, t_shell *sh);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:57:01 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/03 21:35:30 by phonekha         ###   ########.fr       */
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

typedef struct s_cmd
{
	char			**args; //{"ls", "-l", NULL}
	char			*path;  // /bin/ls
	int				infile; // Default 0 (stdin), or FD from <
	int				outfile; // Default 1 (stdout), or FD from >
	bool			piped; // Is there a | after this command ?
	struct s_cmd	*next; // link to the next command in the pipe
} t_cmd;

typedef struct	s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

extern volatile sig_atomic_t	g_signal;

//Utils
void	free2p(char **s);
void	free1p(char **s);

//Main
void	setup_signals(void);
char	**tokenize(char *line);

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



//Execucator
char	**env_to_array(t_env *env);
char	*find_path(char *cmd, t_env *env_list);
int 	exe_cmd(char **args, t_env *env_list);


#endif
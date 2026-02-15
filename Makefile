# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/17 12:54:11 by wintoo            #+#    #+#              #
#    Updated: 2026/02/15 00:48:19 by phonekha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
CC 		= cc
CCFlags = -Wall -Wextra -Werror -g3
LDFlags = -lreadline

SRCS	= srcs/main.c srcs/signals.c srcs/lexer.c srcs/free.c srcs/error.c  srcs/quote_checker.c\
		srcs/mini_executor.c srcs/builtin.c srcs/built_in/mini_cd.c srcs/env_init.c \
		srcs/built_in/mini_echo.c srcs/built_in/mini_env.c srcs/built_in/mini_pwd.c \
		srcs/built_in/mini_unset.c srcs/built_in/mini_export.c srcs/built_in/mini_exit.c \
		srcs/parser.c srcs/parser_utils.c srcs/lexer_utils.c srcs/redirection.c srcs/redir_utils.c srcs/path.c \
		srcs/expand.c srcs/expand_utils.c srcs/env_utils.c srcs/heredoc.c srcs/cmd_execution.c \
		srcs/bonus/wildcards_utils.c srcs/bonus/wildcards.c srcs/syntax.c srcs/input_handler.c \
		srcs/parser_utils_2.c srcs/cmd_execution_utils.c srcs/expand_redir.c\
		srcs/bonus/wildcards_math.c srcs/bonus/wildcards_dir.c

OBJS	= $(SRCS:.c=.o)

RM		= rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CCFlags) $(OBJS) ./libft/libft.a $(LDFlags) -o $@

%.o: %.c
	$(CC) $(CCFlags) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
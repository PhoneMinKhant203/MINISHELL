# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/17 12:54:11 by wintoo            #+#    #+#              #
#    Updated: 2026/02/02 17:14:27 by wintoo           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
CC 		= cc
CCFlags = -Wall -Wextra -Werror -g3
LDFlags = -lreadline

SRCS	= srcs/main.c srcs/signals.c srcs/lexer.c srcs/free.c srcs/error.c \
		srcs/mini_executor.c

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
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/12 19:10:12 by luguimar          #+#    #+#              #
#    Updated: 2024/04/19 05:19:18 by luguimar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = src/main.c src/pipex.c src/pipex_utils.c src/envparser.c src/builtins.c src/envparser_extra.c src/cd_utils.c src/cd_utils1.c src/unset_utils.c src/builtins_extra.c src/errors.c src/cd_utils2.c src/signals.c

OBJS = ${SRC:.c=.o}

CC = cc -g #-fsanitize=address
REMOVE = rm -f
CFLAGS = -Wall -Wextra -Werror
MAKE = make -C
INCLUDE = -I ./include -I ./lib/libft/include
LIBS = -ltermcap -lncurses -lreadline
LIBFT_PATH = lib/libft
LIBFT = ${LIBFT_PATH}/libft.a

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME): ${OBJS} ${LIBFT}
		${CC} ${OBJS} ${INCLUDE} ${LIBS} ${LIBFT} -o ${NAME}

$(LIBFT):
		${MAKE} ${LIBFT_PATH}

all: ${NAME}

clean:
		${MAKE} ${LIBFT_PATH} clean
		${RM} ${OBJS}

fclean: clean
		${MAKE} ${LIBFT_PATH} fclean
		${RM} ${NAME}

re: fclean clean all

.PHONY: all clean fclean re

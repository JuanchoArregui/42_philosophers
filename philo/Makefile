# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/07 16:45:46 by jarregui          #+#    #+#              #
#    Updated: 2025/12/01 12:59:22 by jarregui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# COLOR VARIABLES DECLARATION:
DEL_LINE		=	\033[2K
ITALIC			=	\033[3m
BOLD			=	\033[1m
DEF_COLOR		=	\033[0;39m
GRAY			=	\033[0;90m
RED				=	\033[0;91m
GREEN			=	\033[0;92m
YELLOW			=	\033[0;93m
BLUE			=	\033[0;94m
MAGENTA			=	\033[0;95m
CYAN			=	\033[0;96m
WHITE			=	\033[0;97m
BLACK			=	\033[0;99m
ORANGE			=	\033[38;5;209m
BROWN			=	\033[38;2;184;143;29m
DARK_GRAY		=	\033[38;5;234m
MID_GRAY		=	\033[38;5;245m
DARK_GREEN		=	\033[38;2;75;179;82m
DARK_YELLOW		=	\033[38;5;143m

# VARIABLES DECLARATION:
CC = gcc
CFLAGS = -Wall -Wextra -Werror 
PROY_NAME =	PHILOSOPHERS
EXEC_FILE_NAME = philo
RM = rm -f

# DEBUG_FLAGS =
DEBUG_FLAGS = -g -DDEBUG=1
SANITIZE_FLAGS = -g3 -fsanitize=address

# SOURCES
SRCS 	= philosophers.c \
		philo_utils.c \
		start.c \
		utils.c
OBJS = $(SRCS:.c=.o)

# RULES DECLARATION:
all: philo

# CREACION ARCHIVOS OBJETO
%.o : %.c
	@echo "${ORANGE}${PROY_NAME} compiling file: $(BROWN)[$<]...${DEF_COLOR}"
	@$(CC) $(CFLAGS) -c -o $@ $< 

philo: $(OBJS)
	@$(CC) ${CFLAGS} ${OBJS} -o $(EXEC_FILE_NAME)
	@echo "$(GREEN)✓ Created $(EXEC_FILE_NAME) File$(DEF_COLOR)\n"

debug: fclean
	$(MAKE) CFLAGS="$(CFLAGS) $(DEBUG_FLAGS) $(SANITIZE_FLAGS)" philo
	@echo "$(GREEN)DEBUG: Ejecutando con sanitizer y DEBUG=1$(DEF_COLOR)"
	@echo "$(YELLOW)→ Generando parámetros aleatorios...$(DEF_COLOR)"
	@NP=$$(shuf -i 2-50 -n 1); \
	  TD=$$(shuf -i 200-2000 -n 1); \
	  TE=$$(shuf -i 50-300 -n 1); \
	  TS=$$(shuf -i 50-300 -n 1); \
	  echo "$(CYAN)Parámetros generados: $(YELLOW)$$NP $$TD $$TE $$TS$(DEF_COLOR)"; \
	  ./$(EXEC_FILE_NAME) $$NP $$TD $$TE $$TS

clean:
	@${RM} ${OBJS} ${OBJS_BONUS}
	@echo "\n$(GREEN)✓ All objects from ${PROY_NAME} cleaned successfully$(DEF_COLOR)"

fclean: clean
	@${RM} ${EXEC_FILE_NAME} 
	@echo "\n$(GREEN)✓ All executable from ${PROY_NAME} cleaned successfully $(DEF_COLOR)"

re: fclean all

.PHONY: all clean fclean re

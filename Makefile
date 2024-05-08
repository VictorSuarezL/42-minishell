NAME = minitalk
NAME_BONUS = minitalk_bonus
OBJ_DIR = obj
SRC_DIR = src

# Libft
LIBFT = libft.a
LIBFT_SRC = ./lib/libft/

# Compiler
CC					=	gcc
RM					=	rm -f
HEADERS 			= 	-I ./includes -I $(LIBFT_SRC)
CFLAGS				=	-Wall -Werror -Wextra $(HEADERS)
# Colours
RED					=	\033[0;31m
GREEN				=	\033[0;32m
YELLOW				=	\033[0;33m
BLUE				=	\033[0;34m
PURPLE				=	\033[0;35m
CYAN				=	\033[0;36m
WHITE				=	\033[0;37m
RESET				=	\033[0m

SRC_FILES_SERVER = $(SRC_DIR)/server.c
SRC_FILES_CLIENT = $(SRC_DIR)/client.c
SRC_FILES_SERVER_BONUS = $(SRC_DIR)/server_bonus.c
SRC_FILES_CLIENT_BONUS = $(SRC_DIR)/client_bonus.c

OBJ_SERVER = $(SRC_FILES_SERVER:.c=.o)
OBJ_CLIENT = $(SRC_FILES_CLIENT:.c=.o)
OBJ_SERVER_BONUS = $(SRC_FILES_SERVER_BONUS:.c=.o)
OBJ_CLIENT_BONUS = $(SRC_FILES_CLIENT_BONUS:.c=.o)

# Rules
all: $(NAME)
	@printf "$(BLUE)==> $(CYAN)Compiled\n$(RESET)"

bonus: $(NAME_BONUS)

$(NAME): server client

$(NAME_BONUS): server_bonus client_bonus

$(LIBFT):
	make -C $(LIBFT_SRC)

server: $(LIBFT) $(OBJ_SERVER)
	gcc -o $@ $(OBJ_SERVER) $(CFLAGS) $(LIBFT_SRC)$(LIBFT)

client: $(LIBFT) $(OBJ_CLIENT)
	gcc -o $@ $(OBJ_CLIENT) $(CFLAGS) $(LIBFT_SRC)$(LIBFT)

$(OBJ_SERVER): %.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
	$(info    OBJ_SERVER is $(OBJ_SERVER))

$(OBJ_CLIENT): %.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
	$(info    OBJ_CLIENT is $(OBJ_CLIENT))

server_bonus: $(LIBFT) $(OBJ_SERVER_BONUS)
	gcc -o $@ $(OBJ_SERVER_BONUS) $(CFLAGS) $(LIBFT_SRC)$(LIBFT)

client_bonus: $(LIBFT) $(OBJ_CLIENT_BONUS)
	gcc -o $@ $(OBJ_CLIENT_BONUS) $(CFLAGS) $(LIBFT_SRC)$(LIBFT)

$(OBJ_SERVER_BONUS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
	$(info    OBJ_SERVER_BONUS is $(OBJ_SERVER_BONUS))

$(OBJ_CLIENT_BONUS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
	$(info    OBJ_CLIENT_BONUS is $(OBJ_CLIENT_BONUS))

clean:
	$(RM) $(NAME) $(OBJ_SERVER) $(OBJ_CLIENT) $(OBJ_SERVER_BONUS) $(OBJ_CLIENT_BONUS)
	make -C $(LIBFT_SRC) clean
	@printf "$(BLUE)==> $(RED)Removed\n$(RESET)"

fclean: clean
	rm -rf server client server_bonus client_bonus
	@make -C $(LIBFT_SRC) fclean

re: fclean all
	@printf "$(BLUE)==> $(CYAN)Recompiled\n$(RESET)"

.PHONY: all clean fclean re

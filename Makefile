NAME = minishell
OBJ_DIR = obj
SRC_DIR = src

# Libft
LIBFT = libft.a
LIBFT_SRC = ./lib/libft/

# Compiler
RM					=	rm -f
INCLUDE 			= 	-I ./includes -I $(LIBFT_SRC)
# CFLAGS				=	-Wall -Werror -Wextra 
# CC					=	gcc -g $(CFLAGS) $(INCLUDE)
CC					=	clang -O0 -g -MMD $(INCLUDE)
# Colours
RED					=	\033[0;31m
GREEN				=	\033[0;32m
YELLOW				=	\033[0;33m
BLUE				=	\033[0;34m
PURPLE				=	\033[0;35m
CYAN				=	\033[0;36m
WHITE				=	\033[0;37m
RESET				=	\033[0m

# SRC_FILES 	= $(SRC_DIR)/have_heredoc.c \
# 				$(SRC_DIR)/split_m.c \
# 				$(SRC_DIR)/main.c \
# 				$(SRC_DIR)/validator.c \

# SRC_FILES = $(SRC_DIR)/sh.c
# SRC_FILES = $(SRC_DIR)/search_path.c
SRC_FILES = $(SRC_DIR)/search_path.c \
			$(SRC_DIR)/parse.c \
			$(SRC_DIR)/tokenize.c \
			$(SRC_DIR)/constructor.c \
			$(SRC_DIR)/main.c
# SRC_FILES = $(SRC_DIR)/main.c
# SRC_FILES = $(SRC_DIR)/hhp3.c

OBJ_FILES = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
DEP = $(addsuffix .d, $(basename $(SRC_FILES)))

# Rules
all: $(NAME)

$(LIBFT_SRC)/libft.a:
	make -C $(LIBFT_SRC)
	make bonus -C $(LIBFT_SRC)

$(OBJ_DIR)/%.o: %.c Makefile
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@

$(NAME): $(LIBFT_SRC)/libft.a $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -L$(LIBFT_SRC) -lft -o $(NAME)

-include $(DEP)

clean:
	$(RM) $(OBJ_FILES) $(DEP)
	rm -rf ./$(OBJ_DIR)
	make -C $(LIBFT_SRC) clean
	@printf "$(BLUE)==> $(RED)Removed\n$(RESET)"

fclean: clean
	rm -rf $(NAME) 
	@make -C $(LIBFT_SRC) fclean

re: fclean all
	@printf "$(BLUE)==> $(CYAN)Recompiled\n$(RESET)"

.PHONY: all clean fclean re

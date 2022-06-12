NAME = ircserv
SRCS = src/Socket.cpp
HEADERS_DIR = include

MAIN = main.cpp

OBJS = $(patsubst %.cpp,$(OBJS_DIR)/%.o, $(SRCS))
D_FILES = $(patsubst %.cpp,$(OBJS_DIR)/%.d, $(SRCS))
OBJS_DIR = objs
ALL_OBJS_DIRS = $(sort $(dir $(OBJS)))

CC = clang++ -g
FLAGS = -Wall -Wextra -Werror -std=c++98

#colors for beauty
YELLOW = \033[33;1m
RESET = \033[0m
RED = \033[31;1m
GREEN = \033[32;1m
MAGENTA = \033[35;1m

ERASE = \33[2K

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(ALL_OBJS_DIRS) $(OBJS) $(MAIN)
	@$(CC) $(FLAGS) -I${HEADERS_DIR} $(MAIN) $(OBJS) -o $(NAME)
	@echo "\n$(MAGENTA)$(NAME) $(GREEN)compiled$(RESET)"

$(ALL_OBJS_DIRS): $(OBJS_DIR)
	@mkdir -p $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o:%.cpp
	@$(CC) $(FLAGS) -I${HEADERS_DIR} -c $< -o $@ -MMD
	@printf "$(ERASE)$(RED)>> $(YELLOW)[$@]$(GREEN)$(RESET)\r"

include $(wildcard $(D_FILES))

clean:
	@rm -rf $(OBJS_DIR)
	@echo "$(YELLOW)objs $(RED)deleted$(RESET)"

fclean: clean
	@rm -rf $(NAME) $(NAME).dSYM
	@echo "$(MAGENTA)$(NAME) $(RED)deleted$(RESET)"

re: fclean all

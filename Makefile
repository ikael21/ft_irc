NAME = ircserv
MAIN = main.cpp
HEADERS_DIR = include

SRCS = src/irc_server/IrcServer.cpp \
       src/irc_server/initializers.cpp \
       src/irc_server/events.cpp \
       src/irc_server/helpers.cpp \
       src/irc_server/debug_info_printers.cpp \
       src/Channel.cpp src/User.cpp src/errors.cpp \
       src/Command.cpp src/commands/PASS.cpp \
       src/commands/NICK.cpp src/commands/USER.cpp \
       src/commands/PRIVMSG.cpp src/commands/AWAY.cpp \
       src/commands/NOTICE.cpp src/utils.cpp \
       src/commands/JOIN.cpp src/commands/PONG.cpp \

OBJS = $(patsubst %.cpp,$(OBJS_DIR)/%.o, $(SRCS))
D_FILES = $(patsubst %.cpp,$(OBJS_DIR)/%.d, $(SRCS))
OBJS_DIR = objs
ALL_OBJS_DIRS = $(sort $(dir $(OBJS)))

INCLUDES = -I$(HEADERS_DIR)

OS = $(shell uname)
ifeq ($(OS), Linux)
  INCLUDES += -I/usr/include/kqueue
  LDFLAGS += -lkqueue
  CC = g++
else
  CC = clang++
endif

CC += -g
FLAGS = -Wall -Wextra -Werror -std=c++98
ifeq ($(DEBUG), 1)
  FLAGS += -D DEBUG
endif

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
	@$(CC) $(FLAGS) $(INCLUDES) $(MAIN) $(OBJS) -o $(NAME) -O3 $(LDFLAGS)
	@echo "\n$(MAGENTA)$(NAME) $(GREEN)compiled$(RESET)"

$(ALL_OBJS_DIRS): $(OBJS_DIR)
	@mkdir -p $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o:%.cpp
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@ -MMD -O3
	@printf "$(ERASE)$(RED)>> $(YELLOW)[$@]$(GREEN)$(RESET)\r"

include $(wildcard $(D_FILES))

clean:
	@rm -rf $(OBJS_DIR)
	@echo "$(YELLOW)objs $(RED)deleted$(RESET)"

fclean: clean
	@rm -rf $(NAME) $(NAME).dSYM
	@echo "$(MAGENTA)$(NAME) $(RED)deleted$(RESET)"

re: fclean all

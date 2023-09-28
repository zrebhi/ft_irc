CC = g++
CFLAGS = -g3 -std=c++98 -Wall -Wextra -Werror
SRCS_DIR = src
OBJS_DIR = .obj
NAME = ft_irc

SRCS_LIST = $(wildcard $(SRCS_DIR)/**/*.cpp) $(wildcard $(SRCS_DIR)/*.cpp)
OBJS_LIST = $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(SRCS_LIST))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS_LIST)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all


CXX=c++
CXXFLAGS = -Wall -Werror -Wextra -g -std=c++98 -I include
NAME = webserv
SRC_DIR = source/
LOG_DIR = log/
SRCS = $(SRC_DIR)main.cpp \
	$(SRC_DIR)Logger.cpp \
	$(SRC_DIR)JSON/JSONReader.cpp \
	$(SRC_DIR)JSON/JSONUtils.cpp \
	$(SRC_DIR)Config.cpp

OBJ_DIR = .build/
OBJS = $(SRCS:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@mkdir -p $(LOG_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/JSON
	$(CXX) $(CXXFLAGS) $< -c -o $@

fclean: clean
	rm -f $(NAME)
	rm -rf $(LOG_DIR)

clean:
	rm -rf $(OBJ_DIR)

re: fclean all

.PHONY: all fclean clean re

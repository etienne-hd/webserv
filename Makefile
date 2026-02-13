CXX=c++
CXXFLAGS = -Wall -Werror -Wextra -g -std=c++98 -I include
NAME = webserv
SRC_DIR = source/
LOG_DIR = log/
SRCS = $(SRC_DIR)main.cpp \
	$(SRC_DIR)Logger.cpp \
	$(SRC_DIR)JSON/JSONReader.cpp \
	$(SRC_DIR)JSON/JSONUtils.cpp \
	$(SRC_DIR)Config.cpp \
	$(SRC_DIR)Server/Server.cpp \
	$(SRC_DIR)Server/ServerUtils.cpp \
	$(SRC_DIR)Server/ServerResponse.cpp \
	$(SRC_DIR)Server/ServerRequest.cpp \
	$(SRC_DIR)Server/ServerCGI.cpp \
	$(SRC_DIR)ServerManager.cpp \
	$(SRC_DIR)Method.cpp \
	$(SRC_DIR)utils.cpp \
	$(SRC_DIR)Request.cpp \
	$(SRC_DIR)Response.cpp \
	$(SRC_DIR)Client.cpp \
	$(SRC_DIR)Headers.cpp

OBJ_DIR = .build/
OBJS = $(SRCS:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@mkdir -p $(LOG_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/JSON
	@mkdir -p $(OBJ_DIR)/Server
	$(CXX) $(CXXFLAGS) $< -c -o $@

fclean: clean
	rm -f $(NAME)
	rm -rf $(LOG_DIR)

clean:
	rm -rf $(OBJ_DIR)

re: fclean all

.PHONY: all fclean clean re

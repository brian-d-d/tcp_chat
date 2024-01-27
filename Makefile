C++ = g++
C++FLAGS = -Wall -Werror -Wextra -std=c++2b
INCLUDE = include
BUILD = build
SRC = src
LIB = lib

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp, build/%.o,$(SRCS))

all: $(OBJS)
	$(C++) $(C++FLAGS) $^ -L $(LIB) -l cryptopp -o async_tcp

build/%.o: src/%.cpp
	$(C++) $(C++FLAGS) -I ./$(INCLUDE) -I ./$(LIB) -I ./$(LIB)/built_boost -c $< -o $@ 

clean: 
	rm -rf $(BUILD) && mkdir $(BUILD)

server_main:
	$(C++) $(C++FLAGS) -I ./$(INCLUDE) -I ./$(LIB) -I ./$(LIB)/built_boost -c testing/main.cpp -o testing/main.o

server: all server_main 
	$(C++) $(C++FLAGS) build/async_server_tcp.o build/async_connection_tcp.o testing/main.o -L $(LIB) -lcryptopp -lmysqlcppconn -o testing/server


C++ = g++
C++FLAGS = -Wall -Werror -std=c++23

all: async_client_tcp.o 
opt: options_handler.o

main: options_handler.o main.o
	$(C++) $(C++FLAGS) $^ -o cli_handler.o

%.o: %.cpp
	$(C++) $(C++FLAGS) -c $< -o $@ 

clean: 
	rm *.o

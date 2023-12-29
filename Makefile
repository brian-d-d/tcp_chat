C++ = g++
C++FLAGS = -Wall -Werror -std=c++23

all: async_client_tcp.o 
opt: options_handler.o

# CPP_FILES=$$(ls *.cpp | cut -f 1 -d '.')

# test:
	# @echo $(CPP_FILES)
	
cli_handler: options_handler.o main.o
	$(C++) $(C++FLAGS) $^ -o cli_handler

%.o: %.cpp
	$(C++) $(C++FLAGS) $< -o $@ 

clean: 
	rm *.o

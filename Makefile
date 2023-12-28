C++ = g++
C++FLAGS = -Wall -Werror -std=c++23

all: async_client_tcp.o

%.o: %.cpp
	$(C++) $(C++FLAGS) $< -o $@ 

clean: 
	rm *.o
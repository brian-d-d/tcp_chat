C++ = g++
C++FLAGS = -Wall -Werror -std=c++23

all: async_client_tcp.o main.o options_handler.o rsa_enc_dec.o
	$(C++) $(C++FLAGS) $^ -l:libcryptopp.a -o async_tcp.o


opt: options_handler.o

# CPP_FILES=$$(ls *.cpp | cut -f 1 -d '.')

# test:
	# @echo $(CPP_FILES)

rsa_enc_dec.o: 
	$(C++) -DNDEBUG -g3 -O2 -Wall -Wextra -c -std=c++23 -o  rsa_enc_dec.o rsa_enc_dec.cpp -l:libcryptopp.a
	
cli_handler: options_handler.o main.o
	$(C++) $(C++FLAGS) $^ -o cli_handler.o

%.o: %.cpp
	$(C++) $(C++FLAGS) -c $< -o $@ 

clean: 
	rm *.o


#g++ -DNDEBUG -g3 -O2 -Wall -Wextra -o test cryptopp_testing.cpp -l:libcryptopp.a

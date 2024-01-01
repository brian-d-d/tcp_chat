C++ = g++
C++FLAGS = -Wall -Werror -std=c++2b -I $(BOOST_INCLUDE_PATH) -I $(CRYPTOPP_INCLUDE_PATH) -L $(LIBRARY_PATH) 
LIBRARY_PATH = /usr/local/lib
BOOST_INCLUDE_PATH = ../built_boost/include
CRYPTOPP_INCLUDE_PATH = ../cryptopp/include

all: async_client_tcp.o main.o options_handler.o rsa_enc_dec.o
	$(C++) $(C++FLAGS) $^ -l cryptopp -o async_tcp

opt: options_handler.o
	
cli_handler: options_handler.o main.o
	$(C++) $(C++FLAGS) $^ -o cli_handler.o

%.o: %.cpp
	$(C++) $(C++FLAGS) -c $< -o $@ 

clean: 
	rm *.o

C++ = g++

%.o: %.cpp
	$(C++) $< -o $@ 

clean: 
	rm *.o
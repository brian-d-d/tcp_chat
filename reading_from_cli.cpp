#include <string>
#include <iostream>


int main(int argc, char* argv[]) {
    
    std::cout << argv[1] << std::endl;

    for (std::string line; std::getline(std::cin, line);) {
        std::cout << line << std::endl;
    }


    return 0;
}
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "options_handler.h"

int main(int argc, char* argv[]) {
    std::vector<std::pair<std::string, std::string>> options_v;

    if (handle_options(argc, argv, options_v) != option_code::success) {
        std::cout << "An error occured, exiting..." << std::endl;
        return 1;
    }
    for (std::pair<std::string, std::string> opt_arg : options_v) {
        std::cout << opt_arg.first << " : " << opt_arg.second << std::endl;
    }

    return 0;
}
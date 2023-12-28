#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "options_handler.h"

std::string convert_to_string(char arr[]) {
    std::string str = std::string();
    for (int i = 0; arr[i] != '\0'; i++) {
        str += arr[i]; 
    }
    return str;
}

option_code handle_option(char specifer[], char argument[], std::vector<std::pair<std::string, std::string>>& options_v) {
    std::string specifer_str = convert_to_string(specifer);
    std::string argument_str = convert_to_string(argument);

    if (!specifer_str.contains("-")) {
        std::cout << "Invalid option: " << specifer_str << std::endl;
        return option_code::option_error;
    }
    
    options_v.push_back(std::pair<std::string, std::string>(specifer_str, argument_str));
    return option_code::no_error;
}

option_code handle_options(int option_c, char* options[], std::vector<std::pair<std::string, std::string>>& options_v) {
    for (int i = 1; i <= (option_c - 1); i += 2) { 
        option_code ec = handle_option(options[i], options[i+1], options_v);
        if (ec != option_code::no_error) {
            return option_code::overall_error;
        }
    }
    return option_code::no_error;
}
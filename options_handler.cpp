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

option_code handle_option(char specifier[], char argument[], options_vector& options_v) {
    std::string specifier_str = convert_to_string(specifier);
    std::string argument_str = convert_to_string(argument);

    if (!specifier_str.contains("-")) {
        std::cout << "Invalid option: " << specifier_str << std::endl;
        return option_code::invalid_option;
    }
    
    options_v.push_back(std::pair<std::string, std::string>(specifier_str, argument_str));
    return option_code::success;
}

option_code handle_options(int option_c, char* options[], options_vector& options_v) {
    if (option_c <= 2) {
        std::cout << "No options entered" << std::endl;
        return option_code::not_enough_options;
    }
    for (int i = 1; i <= (option_c - 1); i += 2) { 
        option_code ec = handle_option(options[i], options[i+1], options_v);
        if (ec != option_code::success) {
            return ec;
        }
    }
    return option_code::success;
}
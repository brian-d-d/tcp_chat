#include <iostream>
#include <string>
#include <array>
#include <vector>

enum option_code {
    success,
    invalid_option,
    not_enough_options
};

typedef std::vector<std::pair<std::string, std::string>> options_vector;

std::string convert_to_string(char arr[]);

option_code handle_option(char specifer[], char argument[], options_vector& options_v);

option_code handle_options(int option_c, char* options[], options_vector& options_v);
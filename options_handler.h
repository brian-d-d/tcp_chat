#include <iostream>
#include <string>
#include <array>
#include <vector>

enum option_code {
    success,
    invalid_option,
    invalid_option_count,
    not_enough_options
};

typedef std::vector<std::pair<std::string, std::string>> options_vector;

std::string arr_to_string(char arr[]);

int str_to_int(std::string str);

option_code handle_option(char specifer[], char argument[], options_vector& options_v);

option_code handle_options(int option_c, char* options[], options_vector& options_v);
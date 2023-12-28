#include <iostream>
#include <string>
#include <array>
#include <vector>

enum option_code {
    no_error,
    option_error,
    overall_error
};

std::string convert_to_string(char arr[]);

option_code handle_option(char specifer[], char argument[], std::vector<std::pair<std::string, std::string>>& options_v);

option_code handle_options(int option_c, char* options[], std::vector<std::pair<std::string, std::string>>& options_v);
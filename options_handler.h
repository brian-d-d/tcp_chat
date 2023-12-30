#ifndef __OPTIONS_HANDLER__H
#define __OPTIONS_HANDLER__H

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "async_client_tcp.h"

enum option_code {
    success,
    invalid_option,
    invalid_option_count,
    invalid_option_combination,
    not_enough_options,
};

typedef std::vector<std::pair<std::string, std::string>> string_pair_vector;

std::string arr_to_string(char arr[]);

int str_to_int(std::string str);

option_code handle_option(char specifer[], char argument[], string_pair_vector& options_v);

option_code handle_options(int option_c, char* options[], string_pair_vector& options_v);

option_code pass_options(string_pair_vector& options_vector, tcp_client& tcp_connection);

#endif
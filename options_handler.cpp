#include "options_handler.h"

std::string arr_to_string(char arr[]) {
    std::string str = std::string();
    for (int i = 0; arr[i] != '\0'; i++) {
        str += arr[i]; 
    }
    return str;
}

int str_to_int(std::string str) {
    return std::stoi(str.data());
}

option_code handle_option(char specifier[], char argument[], string_pair_vector& options_vec) {
    std::string specifier_str = arr_to_string(specifier);
    std::string argument_str = arr_to_string(argument);

    if (!specifier_str.contains("-")) {
        std::cout << "Invalid option: " << specifier_str << std::endl;
        return option_code::invalid_option;
    }
    
    options_vec.push_back(std::pair<std::string, std::string>(specifier_str, argument_str));
    return option_code::success;
}

option_code handle_options(int option_c, char* options[], string_pair_vector& options_vec) {
    if (option_c == 1) {
        std::cout << "No options entered" << std::endl;
        return option_code::not_enough_options;
    }
    else if (option_c > 1 && option_c % 2 != 1) {
        std::cout << "Invalid value to argument count" << std::endl;
        return option_code::invalid_option_count;
    }
    for (int i = 1; i <= (option_c - 1); i += 2) { 
        option_code ec = handle_option(options[i], options[i+1], options_vec);
        if (ec != option_code::success) {
            return ec;
        }
    }
    return option_code::success;
}

option_code pass_options(string_pair_vector& options_vector, tcp_client& tcp_connection) {
    std::string host;
    std::string port;
    bool listen;
    bool connect;
    for (std::pair<std::string, std::string> option_argument : options_vector) {
        if (option_argument.first == "-l") {
            port = std::string(option_argument.second);
            listen = true;
        }
        else if (option_argument.first == "-h") {
            host = std::string(option_argument.second);
            connect = true;
        }
        else if (option_argument.first == "-p") {
            port = std::string(option_argument.second);
            connect = true;
        }
    }
    //Listen mode
    if (listen == true && connect == false) {
        tcp_connection.enable_encryption(4096);
        tcp_connection.accept_connection(str_to_int(port));
        return option_code::success;
    }
    //Connect mode
    else if (connect == true && listen == false) {
        tcp_connection.enable_encryption(4096);
        tcp_connection.connect_to(host, port);
        return option_code::success;
    }
    else {
        std::cout << "Invalid combination of arguments" << std::endl;
        return option_code::invalid_option_combination;
    }
}
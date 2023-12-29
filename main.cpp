#include <iostream>
#include <string>
#include <cstdlib>
#include <concepts>
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/posix/basic_descriptor.hpp>
#include "options_handler.h"
#include "async_client_tcp.h"

// int main(int argc, char* argv[]) {
//     std::vector<std::pair<std::string, std::string>> options_v;

//     if (handle_options(argc, argv, options_v) != option_code::success) {
//         std::cout << "An error occured, exiting..." << std::endl;
//         return 1;
//     }
//     for (std::pair<std::string, std::string> opt_arg : options_v) {
//         std::cout << opt_arg.first << " : " << opt_arg.second << std::endl;
//     }

//     return 0;
// }

int main(int argc, char* argv[]) {
    try {
        // if (argc != 3) {
        //     std::cerr << "Usage: client <host> <port>" << std::endl;
        //     return 1;
        // }

        boost::asio::io_context io_context;
        tcp_client client(io_context);
        

        string_pair_vector option_vec(0);

        // std::string temp = "12345";
        // int val = str_to_int(temp);

        // std::cout << val;

        option_code opt_code = handle_options(argc, argv, option_vec);

        if (!opt_code == option_code::success) {
            return 1;
        }
        
        pass_options(option_vec, client);

        boost::asio::executor_work_guard<decltype(io_context.get_executor())> work{io_context.get_executor()};

        io_context.run();
        
        std::cout << "Connection closed" << std::endl;
        
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

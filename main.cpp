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
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;
        
        tcp_client client(io_context, argv[1], argv[2]);

        boost::asio::executor_work_guard<decltype(io_context.get_executor())> work{io_context.get_executor()};

        io_context.run();
        
        std::cout << "Connection closed" << std::endl;
        
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

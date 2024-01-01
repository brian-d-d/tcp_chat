#include <iostream>
#include <string>
#include <cstdlib>
#include <concepts>
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/posix/basic_descriptor.hpp>
#include "options_handler.h"
#include "async_client_tcp.h"
#include "rsa_enc_dec.h"

int main(int argc, char* argv[]) {
    try {
        boost::asio::io_context io_context;
        tcp_client client(io_context);
        
        string_pair_vector option_vec;

        if ((!handle_options(argc, argv, option_vec)) == option_code::success) {
            return 1;
        }

        if ((!pass_options(option_vec, client)) == option_code::success) {
            return 1;
        }
        
        boost::asio::executor_work_guard<decltype(io_context.get_executor())> work{io_context.get_executor()};

        io_context.run();
        
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;

}

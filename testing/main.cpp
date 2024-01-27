#include <iostream>
#include <string>
#include <cstdlib>
#include <concepts>
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/posix/basic_descriptor.hpp>
#include "async_connection_tcp.h"
#include "async_server_tcp.h"

int main() {
    try {
        boost::asio::io_context io_context;

        tcp_server server(io_context);
        
        boost::asio::executor_work_guard<decltype(io_context.get_executor())> work{io_context.get_executor()};

        io_context.run();
        
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;



    
    return 0;
}


#ifndef __ASYNC_CONNECTION_TCP_H
#define __ASYNC_CONNECTION_TCP_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <concepts>
#include "boost/asio.hpp"
#include "utils_tcp.h"
// #include "async_server_tcp.h"

using boost::asio::ip::tcp;

//This will need the enable_shared_from_this thing
//It will then need to to return shared_from_this() and give it to the server


//Functions
//Async read from socket. Callback should pass the contents of the packet to the server
//

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
    public:
        tcp_connection(boost::asio::io_context& io_context, connections_info& connections);

        void read_from_socket();

        tcp::socket& getSocket();

    private:
        tcp::socket _socket;
        boost::asio::streambuf _socket_buffer;
        connections_info& _connections;

        void handle_read_socket(const boost::system::error_code& error, std::size_t bytes_transferred);
};

#endif
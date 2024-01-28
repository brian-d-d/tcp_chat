#ifndef __ASYNC_CONNECTION_TCP_H
#define __ASYNC_CONNECTION_TCP_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <concepts>
#include "boost/asio.hpp"
#include "async_server_tcp.h"
#include "utils_tcp.h"
#include "mysql_connector.h"

using boost::asio::ip::tcp;

//This will need the enable_shared_from_this thing
//It will then need to to return shared_from_this() and give it to the server


//Functions
//Async read from socket. Callback should pass the contents of the packet to the server
//

class tcp_server;

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
    public:
        tcp_connection(boost::asio::io_context& io_context, tcp_server& server, connections_info& connections);

        void read_from_socket();

        void write_to_host(std::string line);

        tcp::socket& getSocket();

        void setConnection(sql::Connection *con);

        std::pair<std::string, std::string> split_data(std::string data);

    private:
        tcp::socket _socket;
        tcp_server& _server;
        boost::asio::streambuf _socket_buffer;
        connections_info& _connections_info;
        sql::Connection *_con;

        void handle_read_socket(const boost::system::error_code& error, std::size_t bytes_transferred);
};

#endif
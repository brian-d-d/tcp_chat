#ifndef __ASYNC_CLIENT_TCP_H
#define __ASYNC_CLIENT_TCP_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <concepts>
#include "boost/asio.hpp"

using boost::asio::ip::tcp;

class tcp_client {
    public: 
        tcp_client(boost::asio::io_context& io_context);

        void connect_to(std::string_view host, std::string_view port);
        
    private:
        boost::asio::io_context& _io_context;
        tcp::socket _socket;
        std::array<char, 128> _stdin_buffer;
        boost::asio::streambuf _socket_buffer;
        boost::asio::posix::stream_descriptor _stdin;

        void read_from_socket();

        void read_from_stdin();

        void write_to_host(std::string line);

        std::string make_time_string();

        void handle_read_socket(const boost::system::error_code& error, std::size_t bytes_transferred);

        void handle_read_stdin(const boost::system::error_code& error, std::size_t bytes_transferred);
};

#endif
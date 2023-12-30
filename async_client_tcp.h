#ifndef __ASYNC_CLIENT_TCP_H
#define __ASYNC_CLIENT_TCP_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <concepts>
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/posix/basic_descriptor.hpp>
#include "rsa_enc_dec.h"

using boost::asio::ip::tcp;

class tcp_client {
    public: 
        tcp_client(boost::asio::io_context& io_context);

        // void enable_encryption(unsigned int key_bits);

        void connect_to(std::string_view host, std::string_view port);
            
        void accept_connection(int port);

        void read_from_socket();

        void read_from_stdin();

        void write_to_host(std::string line);

        std::string make_time_string();
        
    private:
        boost::asio::io_context& _io_context;
        tcp::socket _socket;
        tcp::resolver::results_type _endpoints;
        std::array<char, 128> _stdin_buffer;
        std::array<char, 128> _socket_buffer;
        boost::asio::posix::stream_descriptor _stdin;
        tcp::acceptor _acceptor;
        bool _connection_status;

        void handle_connection(const boost::system::error_code& error);

        void handle_read_socket(const boost::system::error_code& error, std::size_t bytes_transferred);

        void handle_read_stdin(const boost::system::error_code& error, std::size_t bytes_transferred);
};

#endif
//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;


class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
    public:
        typedef std::shared_ptr<tcp_connection> pointer;

        static pointer create(boost::asio::io_context& io_context) {
            return pointer(new tcp_connection(io_context));
        }

        tcp::socket& socket() {
            return _socket;
        }

        void start(std::string message) {
            boost::asio::async_write(_socket, boost::asio::buffer(message),
                std::bind(&tcp_connection::handle_write, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }

    private:
        tcp_connection(boost::asio::io_context& io_context) : _socket(io_context) {

        }

        void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/) {
        
        }

        tcp::socket _socket;
};

class tcp_server {
    public:
        tcp_server(boost::asio::io_context& io_context) : io_context_(io_context), 
        acceptor_(io_context, tcp::endpoint(tcp::v4(), 13)) {
            start_accept();
        }

    private:
        void start_accept() {
            tcp_connection::pointer new_connection = tcp_connection::create(io_context_);

            acceptor_.async_accept(new_connection->socket(),
                std::bind(&tcp_server::handle_accept, this, new_connection,
                boost::asio::placeholders::error));
        }

        void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error) {
            if(!error) {
                new_connection->start("Connected\n");
                for (std::string line; std::getline(std::cin, line);) {
                    new_connection->start(line + "\n");
                }
            }
            start_accept();
        }

        boost::asio::io_context& io_context_;
        tcp::acceptor acceptor_;
};

class tcp_client : public std::enable_shared_from_this<tcp_client> {
    public: 
        tcp_client(boost::asio::io_context& io_context, std::string_view host, std::string_view port) 
        : _io_context(io_context), _socket(io_context) {
            resolve_host(host, port);
            boost::asio::connect(_socket, _endpoints);
        }

        void resolve_host(std::string_view host, std::string_view port) {
            tcp::resolver resolver(_io_context);
            _endpoints = resolver.resolve(host, port);
        }

        void read_from_host() {
            std::array<char, 128> buf;
            boost::system::error_code error;

            while (true) {
                std::array<char, 128> buf;
                boost::system::error_code error;

                size_t len = _socket.read_some(boost::asio::buffer(buf), error);

                if (error == boost::asio::error::eof) {
                    std::cout << "Connection closed by host" << std::endl;
                    break; 
                } 
                else if (error) {
                    throw boost::system::system_error(error); // Some other error.
                }
                std::cout.write(buf.data(), len);
            }
        }

    private:
        boost::asio::io_context& _io_context;
        tcp::socket _socket;
        tcp::resolver::results_type _endpoints;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;
        tcp_server server(io_context);
        io_context.run();


        // tcp_client client(io_context, argv[1], argv[2]);
        // client.read_from_host();

        // for (std::string line; std::getline(std::cin, line);) {
        //     std::cout << line << std::endl;
        // }

    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
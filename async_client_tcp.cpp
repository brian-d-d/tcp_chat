#include <iostream>
#include <string>
#include <cstdlib>
#include <concepts>
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/posix/basic_descriptor.hpp>

using boost::asio::ip::tcp;

class tcp_client {
    public: 
        tcp_client(boost::asio::io_context& io_context, std::string_view host, std::string_view port) :
        _io_context(io_context), 
        _socket(io_context), 
        _stdin(io_context, ::dup(STDIN_FILENO)), 
        _acceptor(io_context, tcp::endpoint(tcp::v4(), 45000)), 
        _connection_status(false) {
            connect_to(host, port);
            
            // accept_connection();
            read_from_socket();
            read_from_stdin();
        }

        void connect_to(std::string_view host, std::string_view port) {
            tcp::resolver resolver(_io_context);
            _endpoints = resolver.resolve(host, port);
            boost::asio::connect(_socket, _endpoints);
            _connection_status = true;
        }

        void accept_connection() {
            _acceptor.async_accept(_socket,
                std::bind(&tcp_client::handle_connection, this,
                boost::asio::placeholders::error));
        }

        void read_from_socket() {
            _socket.async_read_some(boost::asio::buffer(_socket_buffer, _socket_buffer.size()), 
                std::bind(&tcp_client::handle_read_socket, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }

        void read_from_stdin() {
            _stdin.async_read_some(boost::asio::buffer(_stdin_buffer, _stdin_buffer.size()), 
                std::bind(&tcp_client::handle_read_stdin, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }

        void write_to_host(std::string line) {
            boost::asio::write(_socket, boost::asio::buffer(line));
        }
        
    private:
        boost::asio::io_context& _io_context;
        tcp::socket _socket;
        tcp::resolver::results_type _endpoints;
        std::array<char, 128> _stdin_buffer;
        std::array<char, 128> _socket_buffer;
        boost::asio::posix::stream_descriptor _stdin;
        tcp::acceptor _acceptor;
        bool _connection_status;

        void handle_connection(const boost::system::error_code& error) {
            if (_socket.is_open()) {
                _connection_status = true;
                read_from_socket();
                read_from_stdin();
            }
        }

        void handle_read_socket(const boost::system::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                std::string data = std::string(_socket_buffer.data(), bytes_transferred);
                std::cout << data;
                read_from_socket();
            }
            else if (error && _connection_status) {
                _socket.close();
                _io_context.stop();
            }
        }

        void handle_read_stdin(const boost::system::error_code& error, std::size_t bytes_transferred) {
            std::string data = std::string(_stdin_buffer.data(), bytes_transferred);
            if (_socket.is_open()) {
                write_to_host(data);
                read_from_stdin();
            }
        }
};

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
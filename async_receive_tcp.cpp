#include <ctime>
#include <functional>
#include <iostream>
#include <string>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/posix/basic_descriptor.hpp>

using boost::asio::ip::tcp;

class tcp_client {
    public: 
        tcp_client(boost::asio::io_context& io_context, std::string_view host, std::string_view port) :
        _io_context(io_context), _socket(io_context), _stdin(io_context, ::dup(STDIN_FILENO)) {
            resolve_host(host, port);
            boost::asio::connect(_socket, _endpoints);
            read_from_socket();
            read_from_stdin();
        }

        void resolve_host(std::string_view host, std::string_view port) {
            tcp::resolver resolver(_io_context);
            _endpoints = resolver.resolve(host, port);
        }

        void read_from_socket() {
            boost::asio::async_read_until(_socket, _socket_buffer, '\n', 
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
        boost::asio::streambuf _socket_buffer;
        std::array<char, 128> _stdin_buffer;
        boost::asio::posix::stream_descriptor _stdin;

        void handle_read_socket(const boost::system::error_code& error, std::size_t bytes_transferred) {
            std::istream istream(&_socket_buffer);
            std::cout << istream.rdbuf();

            _socket_buffer.consume(_socket_buffer.size());
            read_from_socket();
        }

        void handle_read_stdin(const boost::system::error_code& error, std::size_t bytes_transferred) {
            std::string data = std::string(_stdin_buffer.data(), bytes_transferred);
            write_to_host(data);
            read_from_stdin();
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
        io_context.run();
        
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
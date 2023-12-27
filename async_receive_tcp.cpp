#include <ctime>
#include <functional>
#include <iostream>
#include <string>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

class tcp_client : public std::enable_shared_from_this<tcp_client> {
    public: 
        tcp_client(boost::asio::io_context& io_context, std::string_view host, std::string_view port) 
        : _io_context(io_context), _socket(io_context) {
            resolve_host(host, port);
            boost::asio::connect(_socket, _endpoints);
            read_from_host();
            write_to_host();
        }

        void resolve_host(std::string_view host, std::string_view port) {
            tcp::resolver resolver(_io_context);
            _endpoints = resolver.resolve(host, port);
        }

        void read_from_host() {
                boost::asio::async_read_until(_socket, _buffer, '\n', 
                    std::bind(&tcp_client::handle_read, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }

        void write_to_host() {
            for (std::string line; std::getline(std::cin, line);) {
                // std::cout << line << std::endl;
                boost::asio::async_write(_socket, boost::asio::buffer(line),
                    std::bind(&tcp_client::handle_write, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
                
                if (line == "switch") {
                    std::cout << "hit";
                    read_from_host();
                    return;
                }
            }
        }
        
    private:
        boost::asio::io_context& _io_context;
        tcp::socket _socket;
        tcp::resolver::results_type _endpoints;
        std::array<char, 128> _buf;
        boost::asio::streambuf _buffer;

        void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred) {
            std::istream istream(&_buffer);
            std::cout << istream.rdbuf();

            _buffer.consume(_buffer.size());
            read_from_host();
        }

        void handle_write(const boost::system::error_code& error, std::size_t bytes_transferred) {
        
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
        
        for (std::string line; std::getline(std::cin, line);) {
            std::cout << line << std::endl;
        }

    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
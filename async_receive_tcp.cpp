#include <ctime>
#include <functional>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class tcp_client : public std::enable_shared_from_this<tcp_client> {
    public: 
        tcp_client(boost::asio::io_context& io_context, std::string_view host, std::string_view port) 
        : _io_context(io_context), _socket(io_context) {
            resolve_host(host, port);
            boost::asio::connect(_socket, _endpoints);
            read_from_host();
        }

        void resolve_host(std::string_view host, std::string_view port) {
            tcp::resolver resolver(_io_context);
            _endpoints = resolver.resolve(host, port);
        }

        void read_from_host() {
            // boost::system::error_code error;

            // while (true) {
            //     boost::system::error_code error;

            //     size_t len = _socket.read_some(boost::asio::buffer(_buf), error);

            //     if (error == boost::asio::error::eof) {
            //         std::cout << "Connection closed by host" << std::endl;
            //         break; 
            //     } 
            //     else if (error) {
            //         throw boost::system::system_error(error); // Some other error.
            //     }
            //     std::cout.write(_buf.data(), len);
            // }
            std::string message_ = "write_to";

            boost::asio::async_write(_socket, boost::asio::buffer(message_),
                std::bind(&tcp_client::handle_write, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));

            boost::asio::async_read(_socket, boost::asio::buffer(_buf),
                std::bind(&tcp_client::handle_read, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }

        void handle_read(boost::system::error_code error, std::size_t bytes_transferred) {
            std::cout << "read" << std::endl;
            for (char c : _buf) {
                if (c == 'Z') {
                    return;
                }
                std::cout << c;
            }
        }

        void handle_write(boost::system::error_code error, std::size_t bytes_transferred) {
            std::cout << "write" << std::endl;
        }

    private:
        boost::asio::io_context& _io_context;
        tcp::socket _socket;
        tcp::resolver::results_type _endpoints;
        std::array<char, 128> _buf;
};


int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;
        
        tcp_client client(io_context, argv[1], argv[2]);
        std::cout << "this is after";
        io_context.run();
        
        // client.read_from_host();
        

        // for (std::string line; std::getline(std::cin, line);) {
        //     std::cout << line << std::endl;
        // }

    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
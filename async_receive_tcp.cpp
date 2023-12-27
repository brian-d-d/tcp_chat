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
        


        tcp_client client(io_context, argv[1], argv[2]);
        client.read_from_host();
        io_context.run();

        // for (std::string line; std::getline(std::cin, line);) {
        //     std::cout << line << std::endl;
        // }

    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
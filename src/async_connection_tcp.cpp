#include "async_connection_tcp.h"

tcp_connection::tcp_connection(boost::asio::io_context& io_context) :
    // _io_context(io_context),
    _socket(io_context) {
}

void tcp_connection::read_from_socket() {
    boost::asio::async_read_until(_socket, _socket_buffer, "\n", 
        std::bind(&tcp_connection::handle_read_socket, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

tcp::socket& tcp_connection::getSocket() {
    return _socket;
}

void tcp_connection::handle_read_socket(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
        std::string data = {boost::asio::buffers_begin(_socket_buffer.data()), 
                            boost::asio::buffers_begin(_socket_buffer.data()) + bytes_transferred};

        std::cout << data << std::endl;
        read_from_socket();
    }
}
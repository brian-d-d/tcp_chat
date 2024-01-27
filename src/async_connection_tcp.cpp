#include "async_connection_tcp.h"

tcp_connection::tcp_connection(boost::asio::io_context& io_context, tcp_server& server, connections_info& connections) :
    _socket(io_context),
    _server(server),
    _connections(connections) {
}

void tcp_connection::read_from_socket() {
    boost::asio::async_read_until(_socket, _socket_buffer, "\n", 
        std::bind(&tcp_connection::handle_read_socket, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void tcp_connection::write_to_host(std::string line) {
    boost::asio::write(_socket, boost::asio::buffer(line));
}

tcp::socket& tcp_connection::getSocket() {
    return _socket;
}

void tcp_connection::handle_read_socket(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
        std::string data = {boost::asio::buffers_begin(_socket_buffer.data()), 
                            boost::asio::buffers_begin(_socket_buffer.data()) + bytes_transferred};

        std::cout << data;
        _socket_buffer.consume(bytes_transferred);
        read_from_socket();
    }
    else {
        std::cout << "connection closed" << std::endl;
        _connections.connection_count--;
        _server.close_connection();
    }
    //close the connection if there is an error
}
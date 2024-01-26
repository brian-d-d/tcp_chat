#include "async_server_tcp.h"

tcp_server::tcp_server(boost::asio::io_context& io_context) :
    _io_context(io_context),
    _acceptor(io_context) {
    _acceptor = tcp::acceptor(_io_context, tcp::endpoint(tcp::v4(), 45000));
    accept_connection();
}

void tcp_server::accept_connection() {
    std::shared_ptr<tcp_connection> new_connection{std::make_shared<tcp_connection>(_io_context)};

    _acceptor.async_accept(new_connection->getSocket(),
        std::bind(&tcp_server::handle_connection, this,
        boost::asio::placeholders::error, new_connection));
}

void tcp_server::handle_connection(const boost::system::error_code& error, std::shared_ptr<tcp_connection> connection) {
    if (!error) {
        std::cout << "New connection" << std::endl;
        connection->read_from_socket();
        std::cout << connection->getSocket().remote_endpoint().address() << std::endl; 
        accept_connection();
    }
}
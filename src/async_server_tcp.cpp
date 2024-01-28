#include "async_server_tcp.h"

tcp_server::tcp_server(boost::asio::io_context& io_context, mysqlx::Table& sqltable) :
    _io_context(io_context),
    _acceptor(io_context),
    _connections(),
    _sqltable(sqltable) {
    _acceptor = tcp::acceptor(_io_context, tcp::endpoint(tcp::v4(), 45000));
    accept_connection();
}

void tcp_server::accept_connection() {
    std::shared_ptr<tcp_connection> new_connection{std::make_shared<tcp_connection>(_io_context, *this, _connections, _sqltable)};

    _acceptor.async_accept(new_connection->getSocket(),
        std::bind(&tcp_server::handle_connection, this,
        boost::asio::placeholders::error, new_connection));
}

void tcp_server::handle_connection(const boost::system::error_code& error, std::shared_ptr<tcp_connection> connection) {
    if (!error) {
        _connections.connection_count++;

        std::cout << "New connection: " 
        << connection->getSocket().remote_endpoint().address() << ":" 
        << connection->getSocket().remote_endpoint().port() << std::endl;

        connection->read_from_socket();
        _connections.connections.push_back(connection);

        accept_connection();
    }
}

void tcp_server::close_connection(std::shared_ptr<tcp_connection> connection) {
    for (auto it = _connections.connections.begin(); it != _connections.connections.end();) {
        if (*it == connection) {
            unbind_account(connection->getUsername(), _sqltable);
            _connections.connections.erase(it);
            std::cout << "Remaining connections: " <<_connections.connections.size() << std::endl;
            break;
        }
        else {
            it++;
        }
    }
}
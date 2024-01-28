#include "async_connection_tcp.h"

tcp_connection::tcp_connection(boost::asio::io_context& io_context, tcp_server& server, connections_info& connections, mysqlx::Table& sqltable) :
    _socket(io_context),
    _server(server),
    _connections_info(connections),
    _sqltable(sqltable) {
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

std::pair<std::string, std::string> tcp_connection::split_data(std::string data) {
    std::pair<std::string, std::string> username_message;
    std::string::iterator message_start;
    for (std::string::iterator it = data.begin() + header_info::header_length; it != data.end();) {
        while (*it != ':') {
            username_message.first.push_back(*it);
            it++;
        }
        message_start = it + 1;
        break;
    }
    username_message.second.append(message_start, data.end());
    return username_message;
}

void tcp_connection::handle_read_socket(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
        std::string data = {boost::asio::buffers_begin(_socket_buffer.data()), 
                            boost::asio::buffers_begin(_socket_buffer.data()) + (bytes_transferred - 1)};

        std::pair<std::string, std::string> username_something;

        if ((data[0] - '0') == header_type::username_message) {
            username_something = split_data(data);
        }
        else if ((data[0] - '0') == header_type::username_password) {
            username_something = split_data(data);
            if (check_u_p(username_something.first, username_something.second, _sqltable)) {
                write_to_host("Correct combination\n");
            }
            else {
                write_to_host("Invalid combination\n");
                _socket.close();
            }
        }

        std::cout << username_something.first << " : " << username_something.second << std::endl;

        _socket_buffer.consume(bytes_transferred);
        read_from_socket();
    }
    else {
        std::cout << "connection closed" << std::endl;
        _connections_info.connection_count--;
        _server.close_connection(shared_from_this());
    }
}

// void tcp_connection::setConnection(sql::Connection *con) {
//     _con = con;
// }
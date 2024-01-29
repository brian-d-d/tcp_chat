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

void tcp_connection::write_to_client(std::string line) {
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

        handle_data(data);
        _socket_buffer.consume(bytes_transferred);
        read_from_socket();
    }
    else {
        std::cout << "connection closed" << std::endl;
        _connections_info.connection_count--;
        _server.close_connection(shared_from_this());
    }
}

void tcp_connection::handle_data(std::string data) {
    std::pair<std::string, std::string> username_something = split_data(data);

    if ((data[0] - '0') == header_type::username_message_) {
        std::pair<std::string, int> destination_endpoint_pair = get_account_endpoint(username_something.first, _sqltable);

        if (destination_endpoint_pair.first == "NULL") {
            write_to_client("No one with this username is online\n");
        }
        else {
            boost::asio::ip::address_v4 ip_addr = boost::asio::ip::make_address_v4(destination_endpoint_pair.first);
            boost::asio::ip::basic_endpoint<tcp> destination_endpoint(ip_addr, destination_endpoint_pair.second);
            _server.write_to_client(username_something.second, _username, destination_endpoint);
        }
    }
    else if ((data[0] - '0') == header_type::username_password_) {
        if (check_login_details(username_something.first, username_something.second, _sqltable)) {
            write_to_client("Correct combination\n");
            bind_account(username_something.first, _socket.remote_endpoint().address().to_string(), _socket.remote_endpoint().port(), _sqltable);
            _username = username_something.first;
        }
        else {
            write_to_client("Invalid combination\n");
        }
    }
    else if ((data[0] - '0') == header_type::new_username_password_) {
        if (create_account(username_something.first, username_something.second, _sqltable)) {
            write_to_client("Account created\n");
        }
        else {
            write_to_client("Username in use\n");
        }
    }
    else if ((data[0] - '0') == header_type::sign_out_) {
        if (_username != "") {
            unbind_account(_username, _sqltable);
            _username = "";
            write_to_client("Signed out\n");
        }
        else {
            write_to_client("Not signed in\n");
        }

    }
    else if ((data[0] - '0') == header_type::change_password_) {
        if (_username == username_something.first) {
            change_password(username_something.first, username_something.second, _sqltable);
            write_to_client("Password changed\n");
        }
        else {
            write_to_client("You are not signed in");
        }
    }
    else if ((data[0] - '0') == header_type::delete_account_) {
        if (_username == username_something.first) {
            delete_account(username_something.first, _sqltable);
            _username = "";
            write_to_client("Account deleted\n");
        }
        else {
            write_to_client("Cannot delete account\n");
        }
    }

    std::cout << username_something.first << " : " << username_something.second << std::endl;

}

std::string tcp_connection::getUsername() {
    return _username;
}
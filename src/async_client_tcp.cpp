#include "async_client_tcp.h"

tcp_client::tcp_client(boost::asio::io_context& io_context) :
    _io_context(io_context), 
    _socket(io_context), 
    _stdin(io_context, ::dup(STDIN_FILENO)) { 
}

void tcp_client::connect_to(std::string_view host, std::string_view port) {
    tcp::resolver resolver(_io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    boost::asio::connect(_socket, endpoints);

    std::cout << "Connected to "  
    << _socket.remote_endpoint().address() << ":" 
    << _socket.remote_endpoint().port() 
    << std::endl << std::endl;

    read_from_socket();
    read_from_stdin();
}

void tcp_client::read_from_socket() {
    boost::asio::async_read_until(_socket, _socket_buffer, "\n", 
        std::bind(&tcp_client::handle_read_socket, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void tcp_client::read_from_stdin() {
    _stdin.async_read_some(boost::asio::buffer(_stdin_buffer, _stdin_buffer.size()), 
        std::bind(&tcp_client::handle_read_stdin, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void tcp_client::write_to_host(std::string line) {
    boost::asio::write(_socket, boost::asio::buffer(line));
}

std::string tcp_client::make_time_string() {
  std::time_t now = time(0);
  std::string time_date = ctime(&now);
  std::string time = time_date.substr(11, 8);
  return time;
}

void tcp_client::handle_read_socket(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (_socket.is_open() && !error) {
        std::string data = {boost::asio::buffers_begin(_socket_buffer.data()), 
                            boost::asio::buffers_begin(_socket_buffer.data()) + (bytes_transferred - 1)};

        std::cout << data << std::endl;
        std::cout << "<- in " << make_time_string() << std::endl;
        _socket_buffer.consume(bytes_transferred);
        
        read_from_socket();
    }
    else {
        _socket.close();
        _io_context.stop();
    }
}

void tcp_client::handle_read_stdin(const boost::system::error_code& error, std::size_t bytes_transferred) {
    std::string data = std::string(_stdin_buffer.data(), bytes_transferred);
    if (_socket.is_open() && !error) {

        write_to_host(data);

        std::cout << "-> out " << make_time_string() << std::endl;
        read_from_stdin();
    }
}
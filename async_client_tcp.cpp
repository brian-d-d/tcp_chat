#include "async_client_tcp.h"

tcp_client::tcp_client(boost::asio::io_context& io_context) :
    _io_context(io_context), 
    _socket(io_context), 
    _stdin(io_context, ::dup(STDIN_FILENO)), 
    _acceptor(io_context), 
    _enc_dec(),
    _connection_status(false),
    _their_public_key_received(false),
    _delimiter("\n\r\n\r\n") { 
}

void tcp_client::enable_encryption(unsigned int key_bits) {
    _enc_dec.generate_keys(key_bits);
}

void tcp_client::connect_to(std::string_view host, std::string_view port) {
    tcp::resolver resolver(_io_context);
    _endpoints = resolver.resolve(host, port);
    boost::asio::connect(_socket, _endpoints);
    _connection_status = true;

    std::cout << "Connected to "  
    << _socket.remote_endpoint().address() << ":" 
    << _socket.remote_endpoint().port() 
    << std::endl << std::endl;

    if (_enc_dec.getStatus()) {
        write_to_host(_enc_dec.getPublicKey());
        receive_their_public_key();
    }
    
    read_from_socket();
    read_from_stdin();
}

void tcp_client::accept_connection(int port) {
    _acceptor = tcp::acceptor(_io_context, tcp::endpoint(tcp::v4(), port));
    _acceptor.async_accept(_socket,
        std::bind(&tcp_client::handle_connection, this,
        boost::asio::placeholders::error));

    read_from_socket();
    read_from_stdin();
}

void tcp_client::setDelimiter(std::string delimiter) {
    _delimiter = delimiter;
}

void tcp_client::read_from_socket() {
    boost::asio::async_read_until(_socket, _socket_buffer, boost::regex(_delimiter), 
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

void tcp_client::receive_their_public_key() {
    boost::asio::streambuf their_public_key_streambuf;
    boost::asio::read(_socket, their_public_key_streambuf, boost::asio::transfer_exactly(key_size::rsa_2048));
    std::string their_public_key = {boost::asio::buffers_begin(their_public_key_streambuf.data()), 
                                    boost::asio::buffers_end(their_public_key_streambuf.data())};
    _enc_dec.setTheirPublicKey(their_public_key);
}
    
void tcp_client::handle_connection(const boost::system::error_code& error) {
    if (_socket.is_open()) {
        _connection_status = true;

        std::cout << "Connected to "  
        << _socket.remote_endpoint().address() << ":" 
        << _socket.remote_endpoint().port() 
        << std::endl << std::endl;

        if (_enc_dec.getStatus()) {
            receive_their_public_key();
            write_to_host(_enc_dec.getPublicKey());
        }
        
        read_from_socket();
        read_from_stdin();
    }
}

void tcp_client::handle_read_socket(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
        std::string data = {boost::asio::buffers_begin(_socket_buffer.data()), 
                            boost::asio::buffers_begin(_socket_buffer.data()) + (bytes_transferred - _delimiter.size())};
        if (_enc_dec.getStatus()) {
            std::string decrypted_string = _enc_dec.decrypt_text(data);
            std::cout << decrypted_string << std::endl;
        }
        else {
            std::cout << data << std::endl;
        }
        std::cout << "<- in " << make_time_string() << std::endl;
        _socket_buffer.consume(bytes_transferred);
        
        read_from_socket();
    }
    else if (error && _connection_status) {
        _socket.close();
        _io_context.stop();
    }
}

void tcp_client::handle_read_stdin(const boost::system::error_code& error, std::size_t bytes_transferred) {
    std::string data = std::string(_stdin_buffer.data(), bytes_transferred);
    if (_socket.is_open()) {
        if (_enc_dec.getStatus()) {
            write_to_host(_enc_dec.encrypt_text(data) + _delimiter);
        }
        else if (_delimiter == "\n") {
            write_to_host(data);
        }
        else {
            write_to_host(data + _delimiter);
        }
        std::cout << "-> out " << make_time_string() << std::endl;
        read_from_stdin();
    }
}
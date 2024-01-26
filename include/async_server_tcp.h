#ifndef __ASYNC_SERVER_TCP_H
#define __ASYNC_SERVER_TCP_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <concepts>
#include "boost/asio.hpp"
#include "boost/regex.hpp"
#include "async_connection_tcp.h"

using boost::asio::ip::tcp;

/*
Each connection calls the tcp server write function if it receives something to its socket.
Then the server gets the username it is the message is destined for and server sends it there
When the server receives a new client, it should create a new async_connection_tcp
*/



//Functions
//async read from all sockets
//accept connection 
//close connection
//write to socket
//

class tcp_server {
    public:
        tcp_server(boost::asio::io_context& io_context);

        void accept_connection();

    private:
        boost::asio::io_context& _io_context;
        tcp::acceptor _acceptor;

        void handle_connection(const boost::system::error_code& error, std::shared_ptr<tcp_connection> connection);

};



#endif
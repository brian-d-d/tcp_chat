#ifndef __UTILS_TCP_H
#define __UTILS_TCP_H

#include "boost/asio.hpp"
#include "async_connection_tcp.h"

using boost::asio::ip::tcp;

//Circular include error thing
class tcp_connection;

struct connections_info {
    int connection_count;
    //array of shared pointers to all the connections
    std::vector<std::shared_ptr<tcp_connection>> connections;
};

#endif
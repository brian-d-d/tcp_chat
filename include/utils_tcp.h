#ifndef __UTILS_TCP_H
#define __UTILS_TCP_H

#include "boost/asio.hpp"

struct connections_info {
    int connection_count;
    std::vector<boost::asio::ip::tcp::endpoint> endpoints;
};

#endif
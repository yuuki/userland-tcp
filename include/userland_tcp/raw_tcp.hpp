#ifndef RAW_TCP_HPP
#define RAW_TCP_HPP

#include <sys/socket.h>

#include "basic_raw_protocol.hpp"

namespace userland_tcp {
    typedef basic_raw_protocol<AF_INET, AF_INET6, SOCK_RAW, IPPROTO_TCP> raw_tcp;
}

#endif // RAW_TCP_SOCKET_HPP

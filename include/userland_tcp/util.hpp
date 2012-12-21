#ifndef UTIL_HPP
#define UTIL_HPP

#include "userland_tcp/raw_tcp.hpp"

namespace userland_tcp {

    std::string hostname_resolver(const std::string &hostname, const std::string &hint = "")
    {
        boost::asio::io_service io_service;
        raw_tcp::resolver resolver(io_service);
        raw_tcp::resolver::query squery(hostname, hint);
        raw_tcp::resolver::iterator it = resolver.resolve(squery);
        return it->endpoint().address().to_string();
    }
}

#endif // UTIL_HPP

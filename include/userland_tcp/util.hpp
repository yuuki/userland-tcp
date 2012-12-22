#ifndef UTIL_HPP
#define UTIL_HPP

#include "boost/asio/ip/address.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/tuple/tuple.hpp"

#include "userland_tcp/util.hpp"
#include "userland_tcp/raw_tcp.hpp"


#ifdef ENABLE_LOG
#include <iostream>
#define LOG(expr) (std::clog << "# " << __func__ << ": " << expr << std::endl)
#define LOG_FOR(I,A,B) (for (auto I = A; I < B; ++I) {std::cout << *I << " ";} std::cout << std::endl;)
#else
#define LOG(expr) ((void)0)
#define LOG_FOR(I,A,B) ((void)0)
#endif


namespace userland_tcp {

    typedef boost::tuple<std::string, std::string> addr_pair;
    typedef boost::tuple<int, int> port_pair;

    typedef boost::shared_ptr<unsigned char> payload_t;

    boost::asio::ip::address hostname_resolver(const std::string &hostname, const std::string &hint = "")
    {
        boost::asio::io_service io_service;
        raw_tcp::resolver resolver(io_service);
        raw_tcp::resolver::query squery(hostname, hint);
        raw_tcp::resolver::iterator it = resolver.resolve(squery);
        return it->endpoint().address();
    }
}

#endif // UTIL_HPP

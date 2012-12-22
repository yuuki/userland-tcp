#ifndef PACKET_BUILDER_HPP
#define PACKET_BUILDER_HPP

#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include "userland_tcp/util.hpp"
#include "userland_tcp/raw_tcp_header.hpp"

namespace userland_tcp {
namespace util {

    inline static
    void _build_common_segment(tcp_header& tcp_header,
            const port_pair port, const int window)
    {
        tcp_header.source(port.get<0>());
        tcp_header.dest(port.get<1>());
        tcp_header.seq(rand());
        tcp_header.doff(20/4);
        tcp_header.window(window);
    }

    inline static
    tcp_header syn_segment(const addr_pair addr,
            const port_pair port, const int window, const payload_t& payload)
    {
        tcp_header syn_header;
        _build_common_segment(syn_header, port, window);
        syn_header.syn(true);
        std::string saddr = userland_tcp::hostname_resolver(addr.get<0>()).to_string();
        syn_header.compute_checksum(saddr, addr.get<1>(), payload.get());

        return syn_header;
    }

} // util
} // userland_tcp

#endif // PACKET_BUILDER_HPP

#ifndef PACKET_BUILDER_HPP
#define PACKET_BUILDER_HPP

#include <boost/lexical_cast.hpp>

namespace userland_tcp {
namespace util {

    inline static
    tcp_header syn_segment(std::map<std::string, std::string> &argmap) {
        tcp_header tcp_syn_header;
        tcp_syn_header.source(rand());
        tcp_syn_header.dest(lexical_cast<int>(argmap["port"]));
        tcp_syn_header.seq(rand());
        tcp_syn_header.doff(20/4);
        tcp_syn_header.syn(true);
        tcp_syn_header.window(tcp_header::DEFAULT_WINVAL);
        tcp_syn_header.compute_checksum(iphdr.saddr(), iphdr.daddr());

        return tcp_syn_header;
    }

} // util
} // userland_tcp

#endif // PACKET_BUILDER_HPP

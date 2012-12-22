#include <iostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>

#include "userland_tcp/util.hpp"
#include "userland_tcp/util/segment_builder.hpp"
#include "userland_tcp/raw_tcp.hpp"
#include "userland_tcp/raw_tcp_header.hpp"

#define DEFAULT_PORT       18128
#define DEFAULT_WINDOW     100
#define DEFAULT_SEGMENT    100
#define DEFAULT_PACKET_NUM 10000

boost::optional<int> do_kernel_tcp_client() {
    return boost::none;
}


boost::optional<int> do_raw_tcp_client(const std::string interface,
        const boost::asio::ip::address daddr,
        const int dport, std::map<std::string, int> tcp_params)
{
    userland_tcp::raw_tcp protocol = userland_tcp::raw_tcp::v4();
    if (daddr.is_v4()) {
    } else if (daddr.is_v6()) {
        protocol = userland_tcp::raw_tcp::v6();
    } else {
        return boost::none;
    }

    boost::asio::io_service         io_service;
    userland_tcp::raw_tcp::socket   socket(io_service, protocol);
    userland_tcp::raw_tcp::resolver resolver(io_service);
    userland_tcp::raw_tcp::resolver::query query(protocol, daddr.to_string(), "");
    userland_tcp::raw_tcp::endpoint destination = *resolver.resolve(query);

    // TODO connect
    // TODO interface

    const userland_tcp::addr_pair addr_pair("127.0.0.1", daddr.to_string());
    const userland_tcp::port_pair port_pair(rand(), dport);

    int i = 0;

    // first step
    userland_tcp::payload_t buffer = userland_tcp::payload_t(new unsigned char[tcp_params["segment"]]);
    std::fill(buffer.get(), buffer.get()+sizeof(buffer), 0xff);

    boost::asio::streambuf segment_buffer;
    std::ostream os(&segment_buffer);
    for ( ; i < tcp_params["window"]; ++i) {
        userland_tcp::util::syn_segment(addr_pair, port_pair, tcp_params["window"], buffer).dump();
        os << userland_tcp::util::syn_segment(addr_pair, port_pair, tcp_params["window"], buffer).get_header();
        // LOG_FOR(a, segment_buffer.data(), segment_buffer.size());
        socket.send_to(segment_buffer.data(), destination);
        segment_buffer.consume(segment_buffer.size());
    }

    // second step

    LOG("Complete\n" << i << " packets");

    return 0;
}

int main(int argc, char* argv[]) {
    boost::program_options::options_description opt("Options");

    opt.add_options()
        ("help,h", "show help")
        ("interface,i", boost::program_options::value<std::string>(), "Network Interface name")
        ("kernel,k", boost::program_options::value<std::string>(), "Kernel tcp mode")
        ("port,p", boost::program_options::value<int>(), "destination port number")
        ("window,w", boost::program_options::value<int>(), "window size")
        ("segment,s", boost::program_options::value<int>(), "segment size")
        ("num,n", boost::program_options::value<int>(), "the number of packets");

    try {
        boost::program_options::variables_map vmap;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, opt), vmap);
        boost::program_options::notify(vmap);

        if (vmap.count("help") || ++argv == NULL) {
            std::cerr << opt << std::endl;
            return 0;
        }

        const std::string server_host = std::string(*argv);
        std::map<std::string, int> tcp_params = {};

#ifdef __MACH__
        std::string interface = "en0";
#elif __linux__
        std::string interface = "eth2";
#endif
        if (vmap.count("interface")) {
            interface = vmap["interface"].as<std::string>();
        }

        int port = DEFAULT_PORT;
        if (vmap.count("port")) {
            port = vmap["port"].as<int>();
        }

        tcp_params["window"] = userland_tcp::tcp_header::DEFAULT_WINVAL;
        if (vmap.count("window")) {
            tcp_params["window"] = vmap["window"].as<int>();
        }

        tcp_params["segment"] = DEFAULT_SEGMENT;
        if (vmap.count("segment")) {
            tcp_params["segment"] = vmap["segment"].as<int>();
        }

        tcp_params["num"] = DEFAULT_SEGMENT;
        if (vmap.count("num")) {
            tcp_params["num"] = vmap["num"].as<int>();
        }

        const boost::asio::ip::address daddr = userland_tcp::hostname_resolver(server_host);
        LOG("server host: " << daddr);

        if (vmap.count("kernel")) {
            if (const boost::optional<int> error = do_kernel_tcp_client()) {
                return *error;
            }
        } else {
            if (const boost::optional<int> error = do_raw_tcp_client(interface, daddr, port, tcp_params)) {
                return *error;
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return 0;
}

#include <iostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <boost/program_options.hpp>

#include "userland_tcp/raw_tcp.hpp"


boost::optional<int> do_kernel_tcp_server() {
    return boost::none;
}

boost::optional<int> do_raw_tcp_server() {
    boost::asio::io_service io_service;
    return 0;
}

int main(int argc, char* argv[]) {
    boost::program_options::options_description opt("Options");

    opt.add_options()
        ("help,h", "show help")
        ("interface,i", boost::program_options::value<std::string>(), "Network Interface name")
        ("kernel,k", boost::program_options::value<std::string>(), "Kernel tcp mode")
        ("port,p", boost::program_options::value<int>(), "port number");

    try {
        boost::program_options::variables_map vmap;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, opt), vmap);
        boost::program_options::notify(vmap);

        if (vmap.count("help")) {
            std::cerr << opt << std::endl;
            return 1;
        }

        const int port = vmap["port"].as<int>();
        std::string interface;
        if (vmap.count("interface")) {
            interface = vmap["interface"].as<std::string>();
        }

        std::string daddr = argv[0];
        std::cout << daddr << std::endl;

        if (vmap.count("kernel")) {
            if (const boost::optional<int> error = do_kernel_tcp_server()) {
                return *error;
            }
        } else {
            if (const boost::optional<int> error = do_raw_tcp_server()) {
                return *error;
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return 0;
}

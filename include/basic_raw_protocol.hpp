#ifndef BACIC_RAW_PROTOCOL_HPP
#define BACIC_RAW_PROTOCOL_HPP

template<int Family, int FamilyV6, int Type, int Protocol>
class basic_raw_protocol {

public:

    typedef basic_endpoint<basic_raw_protocol>   endpoint;
    typedef basic_raw_socket<basic_raw_protocol> socket;
    typedef basic_resolver<basic_raw_protocol>   resolver;

    static basic_raw_protocol v4() {
        return basic_raw_protocol(Protocol, Family);
    }

    static basic_raw_protocol v6() {
        return basic_raw_protocol(Protocol, FamilyV6);
    }

    int family() const {
        return family_;
    }

    int type() const {
        return Type;
    }

    int protocol() const {
        return protocol_;
    }

    friend bool operator==(const basic_raw_protocol& p1, const basic_raw_protocol& p2)
    {
        return p1.protocol_ == p2.protocol_ && p1.family_ == p2.family_;
    }

    friend bool operator!=(const basic_raw_protocol& p1, const basic_raw_protocol& p2)
    {
        return p1.protocol_ != p2.protocol_ || p1.family_ != p2.family_;
    }

private:

    explicit basic_raw_protocol(int protocol_id, int protocol_family)
        :  protocol_(protocol_id),
           family_(protocol_family)
    {
    }

    int protocol_;
    int family_;
};

#endif // BACIC_RAW_PROTOCOL_HPP

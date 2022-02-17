#ifndef PROJECTWAAAGH_IPADDRESS_HPP
#define PROJECTWAAAGH_IPADDRESS_HPP

#include "Defines.hpp"
#include <boost/asio/ip/address.hpp>

namespace ProjectWaaagh::Net {
#if BOOST_VERSION >= 106600
    using boost::asio::ip::make_address;
    using boost::asio::ip::make_address_v4;
    inline uint32 address_to_uint(boost::asio::ip::address_v4 const& address) { return address.to_uint(); }
#else
    inline boost::asio::ip::address make_address(char const* str) { return boost::asio::ip::address::from_string(str); }
    inline boost::asio::ip::address make_address(char const* str, boost::system::error_code& ec) { return boost::asio::ip::address::from_string(str, ec); }
    inline boost::asio::ip::address make_address(std::string const& str) { return boost::asio::ip::address::from_string(str); }
    inline boost::asio::ip::address make_address(std::string const& str, boost::system::error_code& ec) { return boost::asio::ip::address::from_string(str, ec); }
    inline boost::asio::ip::address_v4 make_address_v4(char const* str) { return boost::asio::ip::address_v4::from_string(str); }
    inline boost::asio::ip::address_v4 make_address_v4(char const* str, boost::system::error_code& ec) { return boost::asio::ip::address_v4::from_string(str, ec); }
    inline boost::asio::ip::address_v4 make_address_v4(std::string const& str) { return boost::asio::ip::address_v4::from_string(str); }
    inline boost::asio::ip::address_v4 make_address_v4(std::string const& str, boost::system::error_code& ec) { return boost::asio::ip::address_v4::from_string(str, ec); }
    inline uint32 address_to_uint(boost::asio::ip::address_v4 const& address) { return address.to_ulong(); }
#endif
}
#endif //PROJECTWAAAGH_IPADDRESS_HPP

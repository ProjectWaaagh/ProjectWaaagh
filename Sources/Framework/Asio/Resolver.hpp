#ifndef PROJECTWAAAGH_RESOLVER_HPP
#define PROJECTWAAAGH_RESOLVER_HPP

#include "IoContext.hpp"
#include "Optional.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <string>

namespace ProjectWaaagh::Asio {
    class Resolver {
    public:
        explicit Resolver(IoContext& ioContext) : mImpl(ioContext) {}

        Optional<boost::asio::ip::tcp::endpoint> Resolve(boost::asio::ip::tcp const& protocol, std::string const& host, std::string const& service) {
            boost::system::error_code errorCode;
#if BOOST_VERSION >= 106600
            boost::asio::ip::resolver_base::flags flagsResolver = boost::asio::ip::resolver_base::all_matching;
            boost::asio::ip::tcp::resolver::results_type results = mImpl.resolve(protocol, host, service, flagsResolver, errorCode);
            if(results.begin() == results.end() || errorCode)
                return {};

            return results.begin()->endpoint();
#else
            boost::asio::ip::resolver_query_base::flags flagsQuery = boost::asio::ip::tcp::resolver::query::all_matching;
            boost::asio::ip::tcp::resolver::query query(std::move(protocol), std::move(host), std::move(service), flagsQuery);
            boost::asio::ip::tcp::resolver::iterator itr = _impl.resolve(query, ec);
            boost::asio::ip::tcp::resolver::iterator end;
            if (itr == end || ec)
                return {};

            return itr->endpoint();
#endif
        }

    private:
        boost::asio::ip::tcp::resolver mImpl;
    };
}
#endif //PROJECTWAAAGH_RESOLVER_HPP

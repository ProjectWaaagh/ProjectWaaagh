#ifndef PROJECTWAAAGH_STRAND_HPP
#define PROJECTWAAAGH_STRAND_HPP

#include "IoContext.hpp"

#include <boost/asio/strand.hpp>
#if BOOST_VERSION >= 106600
#   include <boost/asio/bind_executor.hpp>
#endif

namespace ProjectWaaagh::Asio {
    class Strand : public IoContextBaseNamespace::IoContextBase::strand {
    public:
        Strand(IoContext& ioContext) : IoContextBaseNamespace::IoContextBase::strand(ioContext) {}
    };

#if BOOST_VERSION >= 106600
    using boost::asio::bind_executor;
#else
    template<typename T>
    inline decltype(auto) bind_executor(Strand& strand, T&& t) {
        return strand.wrap(std::forward<T>(t));
    }
#endif
}

#endif //PROJECTWAAAGH_STRAND_HPP

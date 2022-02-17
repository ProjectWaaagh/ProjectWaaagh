#ifndef PROJECTWAAAGH_IOCONTEXT_HPP
#define PROJECTWAAAGH_IOCONTEXT_HPP

#include <boost/version.hpp>

#if BOOST_VERSION >= 106600
#   include <boost/asio/io_context.hpp>
#   include <boost/asio/post.hpp>
#   define IoContextBaseNamespace boost::asio
#   define IoContextBase io_context
#else
#   include <boost/asio/io_service.hpp>
#   define IoContextBaseNamespace boost::asio
#   define IoContextBase io_service
#endif

namespace ProjectWaaagh::Asio {
    class IoContext {
    public:
        IoContext() : mImpl() {}
        explicit IoContext(int concurrency_hind) : mImpl(concurrency_hind) {}

        operator IoContextBaseNamespace::IoContextBase&() { return mImpl; }
        operator IoContextBaseNamespace::IoContextBase const&() const { return mImpl; }

        std::size_t run() { return mImpl.run(); }
        void stop() { mImpl.stop(); }

#if BOOST_VERSION >= 106600
        boost::asio::io_context::executor_type get_executor() noexcept { return mImpl.get_executor(); }
#endif
    private:
        IoContextBaseNamespace::IoContextBase mImpl;
    };

    template<typename T>
    inline decltype(auto) post(IoContextBaseNamespace::IoContextBase& ioContext, T&& t) {
#if BOOST_VERSION >= 106600
        return boost::asio::post(ioContext, std::forward<T>(t));
#else
        return ioContext.post(std::forward<T>(t));
#endif
    }

    template<typename T>
    inline decltype(auto) get_io_context(T&& ioObject) {
#if BOOST_VERSION >= 106600
        return ioObject.get_executor().context();
#else
        return ioObject.get_io_service();
#endif
    }
}

#endif //PROJECTWAAAGH_IOCONTEXT_HPP

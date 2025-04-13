//
// WSConnection.h
//
// author: Ruan Huipeng
// date : 2025-04-12
//

#ifndef ROC_BASE_NET_WSCONNECTION_H
#define ROC_BASE_NET_WSCONNECTION_H

#include <base/noncopyable.h>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/execution/executor.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast.hpp>
#include <memory>

namespace roc::base::net {

class WSConnection : public roc::base::noncopyable,
                     public std::enable_shared_from_this<WSConnection> {

    using stream_type =
        boost::beast::websocket::stream<boost::asio::ip::tcp::socket>;

  public:
    explicit WSConnection(boost::asio::io_context &io_context,
                          stream_type stream);

    boost::asio::awaitable<boost::beast::flat_buffer> read();
    boost::asio::awaitable<size_t> send(const char *data, size_t len);

  private:
    stream_type ws_stream_; // WebSocket流
    boost::asio::io_context &io_context_;
};

} // namespace roc::base::net

#endif // ROC_BASE_NET_WSCONNECTION_H

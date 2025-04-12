// WSConnection.cc
//
// author: Ruan Huipeng
// date : 2025-04-12
//

#include "WSConnection.h"

// 使用命名空间
namespace roc::base::net {

WSConnection::WSConnection(const boost::asio::io_context &io_context,
                           stream_type stream)
    : io_context_(io_context), ws_stream_(stream) {}

boost::asio::awaitable<boost::beast::flat_buffer> WSConnection::read() {}

boost::asio::awaitable<size_t> WSConnection::send(const char *data,
                                                  size_t len) {
    co_return 0;
}

} // namespace roc::base::net

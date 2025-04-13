// WSConnection.cc
//
// author: Ruan Huipeng
// date : 2025-04-12
//

#include "WSConnection.h"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/beast/core/flat_buffer.hpp>

// 使用命名空间
namespace roc::base::net {

WSConnection::WSConnection(boost::asio::io_context &io_context, stream_type stream)
    : io_context_(io_context),
      ws_stream_(stream) {}

boost::asio::awaitable<boost::beast::flat_buffer> WSConnection::read() {
    using namespace boost;

    beast::flat_buffer buffer;
    auto [ec, _] = co_await ws_stream_.async_read(buffer, beast::net::as_tuple);

    if (ec) {
        // 错误  / 连接关闭
    }

    co_return buffer;
}

boost::asio::awaitable<size_t> WSConnection::send(const char *data, size_t len) {
    auto ex = co_await boost::asio::this_coro::executor;
    co_await boost::asio::post(boost::asio::bind_executor(io_context_, boost::asio::use_awaitable));

    auto res = co_await ws_stream_.async_write(boost::asio::buffer(data, len), beast::net::as_tuple);
    co_await boost::asio::post(boost::asio::bind_executor(ex, boost::asio::use_awaitable));

    co_return res;
}

} // namespace roc::base::net

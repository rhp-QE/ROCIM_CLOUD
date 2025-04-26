// WSServer.cc
//
// author: Ruan Huipeng
// date : 2025-04-12
//

#include "WSServer.h"
#include <boost/beast.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/http/field.hpp>
#include <boost/beast/websocket/stream.hpp>

#include <iostream>

#include "WSConnection.h"
#include "base/Utility.h"

// 使用命名空间
namespace roc::base::net {

// 构造函数
WSServer::WSServer(WSServerConfig config, boost::asio::io_context &io_context)
    : config_(config), io_context_(io_context) {}
// 注册新连接回调
std::shared_ptr<WSServer> WSServer::register_on_new_connection_callback(
    OnNewConnectionCallBackT callback) {
    new_con_callback_ = callback;
    return shared_from_this();
}

// 注册新消息回调
std::shared_ptr<WSServer> WSServer::register_on_receive_message_callback(
    OnReceiveMessgaeCallBackT callback) {
    receive_message_callback_ = callback;
    return shared_from_this();
}

boost::asio::awaitable<void> WSServer::do_session_(boost::beast::websocket::stream<boost::beast::tcp_stream> ws_stream) {
    using namespace boost::asio;
    using namespace boost::beast;

    // Set suggested timeout settings for the websocket
    ws_stream.set_option(websocket::stream_base::timeout::suggested(role_type::server));

    // Set a decorator to change the Server of the handshake
    ws_stream.set_option(websocket::stream_base::decorator(
        [](websocket::response_type& res) {
            res.set(
                http::field::server,
                std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-server-coro");
        }));

    // Accept the websocket handshake
    co_await ws_stream.async_accept();

    // construct WSConnnection
    auto wsconn = std::make_shared<WSConnection>(io_context_, std::move(ws_stream));
    connections_.push_back(wsconn);
    util::SAFE_INVOKE(new_con_callback_, wsconn);

    // do_read_continue
    while(true) {
        auto message = co_await wsconn->read();
        util::SAFE_INVOKE(receive_message_callback_, wsconn, message);
    }
}


// 启动
boost::asio::awaitable<void> WSServer::run() {
    std::cout<<"op"<<std::endl;
    auto ex = co_await boost::asio::this_coro::executor;
    co_await boost::asio::post(boost::asio::bind_executor(io_context_, boost::asio::use_awaitable));

    auto address = boost::beast::net::ip::make_address(config_.host);
    auto port = static_cast<unsigned short>(std::stoi(config_.port));
    boost::beast::net::ip::tcp::endpoint endpoint(address, port);
    auto acceptor = boost::asio::ip::tcp::acceptor(io_context_, endpoint);
    int a = 100;
    while (true) {
        std::cout<<"[wsserver] listen at " << config_.port << std::endl;
        
        auto ws_stream = boost::beast::websocket::stream<boost::beast::tcp_stream>{co_await acceptor.async_accept()};

        boost::asio::co_spawn(io_context_, do_session_(std::move(ws_stream)), boost::asio::detached);
    }

    co_await boost::asio::post(boost::asio::bind_executor(ex, boost::asio::use_awaitable));
    co_return;
}

} // namespace roc::base::net

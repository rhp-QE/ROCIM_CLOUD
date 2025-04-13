//
// WSServer.h
//
// author: Ruan Huipeng
// date : 2025-04-12
//

#ifndef ROC_BASE_NET_WSSERVER_H
#define ROC_BASE_NET_WSSERVER_H

#include "base/noncopyable.h"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <expected>
#include <memory>
#include <string>

namespace roc::base::net {

// -------------------Declare (begin)--------------------
class WSConnection;
// -------------------Declare (end)--------------------

// -------------------callback_type (begin)--------------------
using OnNewConnectionCallBackT =
    std::function<void(std::shared_ptr<WSConnection> con)>;

using OnReceiveMessgaeCallBackT = std::function<void(
    std::shared_ptr<WSConnection> con, boost::beast::flat_buffer message)>;
// -------------------callback_type (end)--------------------

struct WSServerConfig {
    std::string host;
    std::string port;
};

class WSServer : public noncopyable,
                 public std::enable_shared_from_this<WSServer> {
  public:
    // 构造函数
    explicit WSServer(WSServerConfig config,
                      const boost::asio::io_context &io_context);

    // 注册新连接回调
    std::shared_ptr<WSServer>
    register_on_new_connection_callback(OnNewConnectionCallBackT callback);

    // 注册新消息回调
    std::shared_ptr<WSServer>
    register_on_receive_message_callback(OnReceiveMessgaeCallBackT callback);

    // 启动
    boost::asio::awaitable<std::expected<bool, std::string>> run();

  private:
    const WSServerConfig config_;
    const boost::asio::io_context &io_context_;
    OnNewConnectionCallBackT new_con_callback_;
    OnReceiveMessgaeCallBackT receive_message_callback_;
    std::vector<std::shared_ptr<WSConnection>> connections_;
};

} // namespace roc::base::net

#endif // ROC_BASE_NET_WSSERVER_H

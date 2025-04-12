// WSServer.cc
//
// author: Ruan Huipeng
// date : 2025-04-12
//

#include "WSServer.h"
#include <boost/asio/io_context.hpp>

// 使用命名空间
namespace roc::base::net {

// 构造函数
WSServer::WSServer(WSServerConfig config,
                   const boost::asio::io_context &io_context)
    : config_(config), io_context_(io_context) {}

} // namespace roc::base::net

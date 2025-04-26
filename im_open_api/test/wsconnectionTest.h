#include <base/net/ws/WSServer.h>
#include <base/net/ws/WSConnection.h>
#include <boost/asio/awaitable.hpp>
#include <boost/beast/core/flat_buffer.hpp>

boost::asio::awaitable<void> onNewMessage(std::shared_ptr<WSConnection> con, boost::beast::flat_buffer message) {
	
}

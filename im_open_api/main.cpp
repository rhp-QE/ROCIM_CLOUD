#include "base/net/ws/WSConnection.h"
#include <boost/exception/exception.hpp>
#include <expected>
#include <iostream>
#include <base/net/ws/WSServer.h>
#include <thread>
#include <boost/asio/detached.hpp>
#include <boost/asio/co_spawn.hpp>
#include <test/SelfCRTPTest.h>

using namespace std;


int main() {

    boost::asio::io_context net_io_context;
    auto worker = boost::asio::make_work_guard(net_io_context);
    std::thread([&]{
        net_io_context.run();
    }).detach();


    boost::asio::io_context main_io_context;
    auto worker1 = boost::asio::make_work_guard(main_io_context);

    selfCRTPTest();
    // -----------------------------------
    auto server = std::make_shared<roc::base::net::WSServer>(roc::base::net::WSServerConfig{"127.0.0.1", "8182"}, net_io_context);

    server -> register_on_new_connection_callback([](std::shared_ptr<roc::base::net::WSConnection> con) {
        int a = 100;
    }) -> register_on_receive_message_callback([&net_io_context](std::shared_ptr<roc::base::net::WSConnection> conn, boost::beast::flat_buffer message){
        // conn->send((const char *)message.data().data(), message.size());
        boost::asio::co_spawn(net_io_context, conn->send("ok", 2), boost::asio::detached);
    });
    boost::asio::co_spawn(net_io_context, server->run(), boost::asio::detached);
    // -----------------------------------

    main_io_context.run();
    
    return 0;
}

#ifndef IM_BASE_CONFIG_H
#define IM_BASE_CONFIG_H

namespace boost::asio {

class io_context;

}


namespace roc {

extern boost::asio::io_context net_io_context;
extern boost::asio::io_context main_io_context;

};

#endif

//
// LCService.h
//
// author: Ruan Huipeng
// date : 2025-04-26
// 

#ifndef IM_OPEN_API_LC_LCSERVICE_H
#define IM_OPEN_API_LC_LCSERVICE_H

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>

namespace im_open_api::lc {

boost::asio::awaitable<void> init_service(boost::asio::io_context& io_context);

} // namespace im_open_api::lc

#endif // IM_OPEN_API_LC_LCSERVICE_H

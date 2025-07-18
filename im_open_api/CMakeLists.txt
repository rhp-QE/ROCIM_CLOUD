cmake_minimum_required(VERSION 3.28.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_program(CMAKE_C_COMPILER gcc-14)
find_program(CMAKE_CXX_COMPILER g++-14)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS_OUTPUT_PATH "../")
set (CMAKE_TOOLCHAIN_FILE "/root/vcpkg/scripts/buildsystems/vcpkg.cmake")


# -------------------------------------
project(ROCIM_CLIENT VERSION 0.1.0 LANGUAGES C CXX)

# -------------------------------------c

include_directories(${CMAKE_SOURCE_DIR})

# 使用更安全的源文件收集方式（推荐显式列出）
file(GLOB_RECURSE SOURCES 
  "base/*.cpp"
  "base/*.cc"
  "im_open_api/*.cc"
  "im_open_api/*.cpp"
  "im_message_api/*.cc"
  "im_message_api/*.cpp"
  "im_conversation_api/*.cc"
  "im_conversation_api/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

add_executable(main ${SOURCES})

#--------------------------------------

# WCDB
target_include_directories(main PRIVATE "/root/ccdep/src/wcdb/src/build/export_headers")
target_link_directories(main PRIVATE "/root/ccdep/src/wcdb/src/build/")
target_link_libraries(main PRIVATE WCDB)

# BOOST
# --------------------------------------------
#set(Boost_DIR "/root/ccdep/boost_1_84_0/lib/cmake/Boost-1.84.0")
#set(BOOST_ROOT "/root/ccdep/boost_1_84_0")

find_package(Boost REQUIRED COMPONENTS asio json system)  # 合并查找
target_link_libraries(main PRIVATE 
    Boost::asio
    Boost::json 
    Boost::system
)
# --------------------------------------------

#protobuf
# target_include_directories(main PRIVATE "root/ccdep/protobuf_3_21_12/include")
# target_link_directories(main PRIVATE "root/ccdep/protobuf_3_21_12/lib")
# target_link_libraries(main PRIVATE protoc protobuf protobuf-lite)

find_package(protobuf CONFIG REQUIRED)
target_link_libraries(main PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

find_package(absl REQUIRED)
target_link_libraries(main PRIVATE absl::absl_log)

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(main PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

# 设置默认构建类型为 Debug（可选）
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 显式添加调试编译选项（通常 CMake 会自动处理）
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")


include(CTest)
enable_testing()


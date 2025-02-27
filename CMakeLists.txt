cmake_minimum_required(VERSION 3.10)

project(MockServer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LZ4 REQUIRED liblz4)

add_executable(MockCollector 
    src/main_collector.cpp
    src/collector/server/Server.cpp
    src/collector/server/MockServer.cpp
    src/collector/server/BaseServer.cpp
    src/utils/Lz4Utils.cpp
) 

target_link_libraries(MockCollector PRIVATE ${LZ4_LIBRARIES})



add_executable(MockBroker 
    src/main_broker.cpp
    src/broker/Server.cpp
)

add_executable(MockClient
    src/main_client.cpp
    src/client/Client.cpp
)

target_link_libraries(MockBroker PRIVATE ${LZ4_LIBRARIES})

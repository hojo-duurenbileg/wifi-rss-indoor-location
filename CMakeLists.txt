cmake_minimum_required(VERSION 3.10)
project(wifi_rss_fingerprinting CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(wifi_rss src/main.cpp src/WiflyInterface.cpp)
target_include_directories(wifi_rss PRIVATE include)

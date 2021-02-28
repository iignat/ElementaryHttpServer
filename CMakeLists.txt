cmake_minimum_required(VERSION 2.8)

project(ElementaryHttpServer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("-std=c++11")
add_executable(final main.cpp server.cpp worker.cpp httpreqparser.cpp httprspcompiler.cpp processor.cpp)

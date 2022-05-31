cmake_minimum_required(VERSION 2.8)

project(ElementaryHttpServer)
include_directories("src")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("-std=c++11")
add_executable(ElementaryServer src/main.cpp src/server.cpp src/worker.cpp src/httpreqparser.cpp src/httprspcompiler.cpp src/processor.cpp)

cmake_minimum_required(VERSION 3.26)
project(JSON_Parser)

set(CMAKE_CXX_STANDARD 14)

add_executable(JSON_Parser
        json.cpp
        test_json.cpp)

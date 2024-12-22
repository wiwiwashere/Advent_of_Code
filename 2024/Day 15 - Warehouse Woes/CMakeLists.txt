cmake_minimum_required(VERSION 3.29)
project(Day_15)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR "D:/CODING/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(Day_15 warehouse_woes.cpp)
target_link_libraries(Day_15 sfml-graphics sfml-audio)

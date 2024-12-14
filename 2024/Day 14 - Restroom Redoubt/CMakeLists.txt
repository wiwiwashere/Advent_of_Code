cmake_minimum_required(VERSION 3.29)
project(Day_14)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR "D:/CODING/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(Day_14 restroom_redoubt.cpp)
target_link_libraries(Day_14 sfml-graphics sfml-audio)


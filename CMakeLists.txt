cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(TDL2048)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

set(SOURCES lib/Network.cpp lib/Network.h)


add_executable(TDL2048 TDL2048.cpp ${SOURCES})
target_link_libraries(TDL2048 "${TORCH_LIBRARIES}")
set_property(TARGET TDL2048 PROPERTY CXX_STANDARD 14)
target_include_directories(TDL2048 PRIVATE lib)
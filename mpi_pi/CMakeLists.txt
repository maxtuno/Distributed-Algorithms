cmake_minimum_required(VERSION 3.13)
project(mpi_pi)

set(CMAKE_CXX_FLAGS "-std=c++17 -Os -Wall -mtune=native")
set(CMAKE_CX_COMPILER mpicxx)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

find_package(Boost REQUIRED COMPONENTS mpi)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(/usr/local/lib)

add_executable(mpi_pi mpi_pi.cc)

target_link_libraries(mpi_pi ${Boost_LIBRARIES} mpi)
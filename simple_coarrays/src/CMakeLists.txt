cmake_minimum_required(VERSION 3.0)
project(simple LANGUAGES Fortran)

set(CMAKE_Fortran_FLAGS "-std=f2008 -fcoarray=lib")
set(CMAKE_Fortran_COMPILER gfortran)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set_source_files_properties(main.f08 PROPERTIES LANGUAGE Fortran)

add_executable(simple main.f08 io.f08)
target_link_libraries(simple caf_mpi)
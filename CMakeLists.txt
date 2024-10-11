#1.cmake verson
cmake_minimum_required(VERSION 2.8)

#2.project name
project(nwordseg)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SRC_LIST "src/*.cpp")

#add lib
add_library(nseg SHARED ${SRC_LIST})

set(CRFPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ncrfpp/include)
set(CRFPP_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ncrfpp/lib)

target_include_directories(nseg PRIVATE ${CRFPP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(nseg PRIVATE ${CRFPP_LIBRARY_DIR}/libcrfpp.so)

#5.generate executable
add_executable(nseg_demo demo/seg_demo.cpp)

target_include_directories(nseg_demo PRIVATE ${CRFPP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(nseg_demo PRIVATE nseg ${CRFPP_LIBRARY_DIR}/libcrfpp.so)


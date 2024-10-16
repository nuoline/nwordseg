cmake_minimum_required(VERSION 3.10)
project(NWordSeg)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加include目录
include_directories(include nseg_model/ncrfpp/include)

# 查找源文件
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# 添加共享库
add_library(nseg SHARED ${SRC_FILES})

# 设置库路径
link_directories(${CMAKE_SOURCE_DIR}/nseg_model/ncrfpp/lib)

# 链接 CRF++ 库
target_link_libraries(nseg "${CMAKE_SOURCE_DIR}/nseg_model/ncrfpp/lib/libcrfpp.so")

# 设置 RPATH 为相对路径
set_target_properties(nseg PROPERTIES
    INSTALL_RPATH "\$ORIGIN/../ncrfpp/lib"
    BUILD_WITH_INSTALL_RPATH TRUE
)

# 添加可执行文件
add_executable(nseg_demo demo/seg_demo.cpp)
# 链接nseg库到demo可执行文件
target_link_libraries(nseg_demo nseg)

# 指定输出目录
set_target_properties(nseg PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/nseg_model/lib"
)

set_target_properties(nseg_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/nseg_model/bin"
)

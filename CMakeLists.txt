mum_required(VERSION 3.10)
project(NWordSeg)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加include目录
include_directories(include nseg_model/ncrfpp/include)

# 设置RPATH
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "$ORIGIN/nseg_model/ncrfpp/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# 查找源文件
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# 创建动态库
add_library(nseg SHARED ${SRC_FILES})

# 链接CRF++库
target_link_libraries(nseg "${CMAKE_SOURCE_DIR}/nseg_model/ncrfpp/lib/libcrfpp.so")

# 添加可执行文件
add_executable(nseg_demo demo/seg_demo.cpp)

# 链接nseg库到nseg_demo可执行文件
target_link_libraries(nseg_demo nseg)

# 指定输出目录
set_target_properties(nseg PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/nseg_model/lib"
)

set_target_properties(nseg_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/nseg_model/bin"
)

# 安装规则
install(TARGETS nseg
    LIBRARY DESTINATION lib
)

install(TARGETS nseg_demo
    RUNTIME DESTINATION bin
)

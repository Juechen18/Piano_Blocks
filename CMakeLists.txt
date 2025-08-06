cmake_minimum_required(VERSION 3.10)

# 设置交叉编译
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
# 这里指定的路径必须是绝对路径
set(CMAKE_C_COMPILER /usr/local/arm/6.5.0/bin/arm-linux-gcc)
set(CMAKE_CXX_COMPILER /usr/local/arm/6.5.0/bin/arm-linux-g++)

project(lvgl)

set(CMAKE_C_STANDARD 99)#C99 # lvgl officially support C99 and above
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# # 包含自定义头文件目录
include_directories(
    ${PROJECT_SOURCE_DIR}/my_code/inc
)


# 修改括号!
add_subdirectory(lvgl )
target_include_directories(lvgl PUBLIC ${PROJECT_SOURCE_DIR} )

# 收集源文件
file(GLOB_RECURSE MY_SOURCES
    ${PROJECT_SOURCE_DIR}/my_code/src/*.c
)


# 添加主可执行文件
add_executable(
    main 
    main.c 
    mouse_cursor_icon.c
    ${MY_SOURCES}
)


target_link_libraries(main lvgl lvgl::examples lvgl::demos lvgl::thorvg ${SDL2_LIBRARIES} m pthread)
add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main DEPENDS main)

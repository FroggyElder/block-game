cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_COMPILER arm-linux-g++ -std=c++17)
set(CMAKE_C_COMPILER arm-linux-gcc)

project(arm_demo_c++)

# set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(Obj demo)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/lcd)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/touch_screen)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/screenObj)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/block)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/menu)


include_directories(${PROJECT_SOURCE_DIR}/lib/lcd)
include_directories(${PROJECT_SOURCE_DIR}/lib/lcd/spng)
include_directories(${PROJECT_SOURCE_DIR}/lib/lcd/font)

include_directories(${PROJECT_SOURCE_DIR}/lib/touch_screen)

include_directories(${PROJECT_SOURCE_DIR}/src/screenObj)
include_directories(${PROJECT_SOURCE_DIR}/src/block)
include_directories(${PROJECT_SOURCE_DIR}/src/menu)
include_directories(${PROJECT_SOURCE_DIR}/src/observer)


add_executable(${Obj} main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(${Obj} lcd touch_screen SObj block menu Threads::Threads m)

add_custom_command(
    TARGET ${Obj}
    POST_BUILD
    COMMAND scp -O ${CMAKE_CURRENT_BINARY_DIR}/${Obj} root@192.168.172.115:/root || true
    COMMENT "copying to arm device"
)
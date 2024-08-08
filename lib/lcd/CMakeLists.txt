set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-pointer-arith")

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/lcd/spng)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/lcd/font)

include_directories(${PROJECT_SOURCE_DIR}/lib/lcd/spng)
include_directories(${PROJECT_SOURCE_DIR}/lib/lcd/font)

add_library(lcd lcd.cpp)

target_link_libraries(lcd spng font)
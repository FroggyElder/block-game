include_directories(${PROJECT_SOURCE_DIR}/src/screenObj)
include_directories(${PROJECT_SOURCE_DIR}/lib/lcd)
include_directories(${PROJECT_SOURCE_DIR}/lib/lcd/spng)
include_directories(${PROJECT_SOURCE_DIR}/lib/lcd/font)

add_library(block block.cpp)
target_link_libraries(block SObj lcd)
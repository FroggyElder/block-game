add_library(menu menu.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src/screenObj)

include_directories(${PROJECT_SOURCE_DIR}/lib/lcd)
include_directories(${PROJECT_SOURCE_DIR}/lib/lcd/spng)
include_directories(${PROJECT_SOURCE_DIR}/lib/lcd/font)

include_directories(${PROJECT_SOURCE_DIR}/lib/touch_screen)
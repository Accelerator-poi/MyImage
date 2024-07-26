cmake_minimum_required(VERSION 3.5)
project(MyImage LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/home/luffy/Qt/6.5.3/gcc_64/" CACHE PATH "Qt Kit Dir")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 修正拼写错误
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -platform xcb")

find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include ${Qt6Core_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS}  )

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE UIS "${CMAKE_SOURCE_DIR}/ui/*.ui")

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${UIS}
    ${HEADERS}
)

# 确保 UI 文件被正确处理
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets ${OpenCV_LIBS})
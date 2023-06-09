cmake_minimum_required(VERSION 3.23)

project(hello-world-cmake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

include_directories(
        ${PROJECT_SOURCE_DIR}/inc
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB PROJECT_SOURCES
        "${PROJECT_SOURCE_DIR}/inc/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})
install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)


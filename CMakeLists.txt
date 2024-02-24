cmake_minimum_required(VERSION 3.23)

project(demo-app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

include_directories(
        ${PROJECT_SOURCE_DIR}/inc
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB PROJECT_SOURCES
        "${PROJECT_SOURCE_DIR}/inc/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDBUSPLUSPLUS sdbusplus REQUIRED)
include_directories(${SDBUSPLUSPLUS_INCLUDE_DIRS})
link_directories(${SDBUSPLUSPLUS_LIBRARY_DIRS})
find_program(SDBUSPLUSPLUS sdbus++)

pkg_check_modules(Boost Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS}) 
link_directories(${Boost_LIBRARY_DIRS})


add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries (${CMAKE_PROJECT_NAME} ${SDBUSPLUSPLUS_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)

pkg_get_variable(SYSTEMD_SERVICE_DIR systemd systemdsystemunitdir)
set(TARGET_SERVICE ${PROJECT_SOURCE_DIR}/xyz.openbmc_project.demoApp.service)
install(FILES ${TARGET_SERVICE} DESTINATION ${SYSTEMD_SERVICE_DIR})

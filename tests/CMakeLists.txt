cmake_minimum_required(VERSION 3.16)

project(demo-app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_BINARY ${CMAKE_PROJECT_NAME}_tst)

FILE(GLOB TARGET_HPP "${CMAKE_SOURCE_DIR}/*.hpp")
FILE(GLOB TARGET_CPP "${CMAKE_SOURCE_DIR}/*.cpp")
SET(TARGET_SOURCES ${TARGET_CPP} ${TARGET_HPP})


include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
	sdbusplus
	GIT_REPOSITORY https://github.com/openbmc/sdbusplus.git
	GIT_TAG        origin/master
)
FetchContent_MakeAvailable(sdbusplus)

FetchContent_Declare(
	boost
	GIT_REPOSITORY https://github.com/boostorg/boost.git
	GIT_TAG        boost-1.82.0
)
FetchContent_MakeAvailable(boost)

FetchContent_Declare(
	systemd
	GIT_REPOSITORY https://github.com/systemd/systemd.git
	GIT_TAG        v255
)
FetchContent_MakeAvailable(systemd)

# ignore BOOST deprecated headers
add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")
include_directories(../src)
include_directories(${sdbusplus_SOURCE_DIR}/include)
include_directories(${systemd_SOURCE_DIR})
add_executable(${TARGET_BINARY} ${TARGET_SOURCES})
target_link_libraries(${TARGET_BINARY} PUBLIC gtest gmock Boost::asio Boost::callable_traits)


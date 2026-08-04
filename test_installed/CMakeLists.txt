cmake_minimum_required(VERSION 3.14)
project(XNetworkInstalledTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find dependencies first - XNetworkConfig.cmake references fmt::fmt and Py2Cpp::Py2Cpp
find_package(fmt REQUIRED)
find_package(Py2Cpp REQUIRED)
find_package(XNetwork REQUIRED)

add_executable(xnetwork_installed_test main.cpp)
target_link_libraries(xnetwork_installed_test PRIVATE XNetwork::XNetwork fmt::fmt Py2Cpp::Py2Cpp)

enable_testing()
add_test(NAME xnetwork_installed_test COMMAND xnetwork_installed_test)

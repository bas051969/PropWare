cmake_minimum_required(VERSION 3.3)
find_package(PropWare REQUIRED)

project(PropWareI2C_Demo)

create_simple_executable(${PROJECT_NAME} PropWareI2C_Demo.cpp)

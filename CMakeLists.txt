cmake_minimum_required (VERSION 3.9)
project(CppMicroserviceExampleGenerator)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PRIVATE include)
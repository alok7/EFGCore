cmake_minimum_required(VERSION 3.20)

project(Test VERSION 0.1)

set(EXECUTABLE ${PROJECT_NAME}-bin)
set(DEFAULT_BUILD_TYPE "Release")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set compiler flags
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS} -O3")

add_executable(${EXECUTABLE} ${${PROJECT_NAME}_SRC} main.cpp)

find_package(EFGCore REQUIRED)
target_link_libraries(${EXECUTABLE} PRIVATE EFGCore)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${EXECUTABLE} PRIVATE Threads::Threads)

find_package(OpenSSL REQUIRED)
target_link_libraries(${EXECUTABLE} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

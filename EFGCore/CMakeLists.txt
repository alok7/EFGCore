cmake_minimum_required(VERSION 3.20)

project(EFGCore LANGUAGES CXX)
include(CMakePackageConfigHelpers)
# make cache variables for install destinations
include(GNUInstallDirs)

set(DEFAULT_BUILD_TYPE "Release")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS} -O3")

set(${PROJECT_NAME}_SRC
            src/Core/Transport/HTTPRequest/HTTPConfig.cpp
            src/Core/Transport/HTTPRequest/HTTPRequest.cpp
            src/Core/Transport/WSS/WSS.cpp
	    src/Core/Transport/SHM/Publisher.cpp
	    src/Core/Transport/SHM/Subscriber.cpp
            src/Core/Logger/Logger.cpp
            src/Core/RateLimit/RateController.cpp
            src/Core/MarketUtils/MarketUtils.cpp
            src/Core/Message/Message.cpp
            src/Core/Message/InstrumentCenter.cpp
            src/Core/Message/TradeCenter.cpp
            src/Core/Message/TickerCenter.cpp
	    src/Core/Message/PriceDepthCenter.cpp
	    src/Core/Message/AggregateTradeCenter.cpp
	    src/Core/Lock/SpinLock.cpp
            )

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SRC} )

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>" "$<INSTALL_INTERFACE:${PROJECT_NAME}/${CMAKE_INSTALL_INCLUDEDIR}>"
                                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

find_package(CURL REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${CURL_INCLUDE_DIRS})
target_link_Libraries(${PROJECT_NAME} PRIVATE ${CURL_LIBRARIES})

find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

#find_package(ZLIB REQUIRED)
#target_include_directories(${PROJECT_NAME} PRIVATE ${ZLIB_INCLUDE_DIRS})
#target_link_Libraries(${PROJECT_NAME} PRIVATE ${ZLIB_LIBRARIES})

target_link_libraries(${PROJECT_NAME} PRIVATE uWS)
target_include_directories(${PROJECT_NAME} PRIVATE "/usr/include/uWS")

target_link_libraries(${PROJECT_NAME} PUBLIC rt)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_TARGETS
        LIBRARY DESTINATION ${PROJECT_NAME}/${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${PROJECT_NAME}/${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${PROJECT_NAME}/${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${PROJECT_NAME}/${CMAKE_INSTALL_INCLUDEDIR}
)


install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${PROJECT_NAME}/${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ${PROJECT_NAME}_TARGETS
        FILE ${PROJECT_NAME}_TARGETS.cmake
        DESTINATION ${PROJECT_NAME}/${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)


configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${PROJECT_NAME}/${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

set(version 1.0.0)
set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${version})
set_property(TARGET ${PROJECT_NAME} PROPERTY SOVERSION 1)
set_property(TARGET ${PROJECT_NAME} PROPERTY
  INTERFACE_${PROJECT_NAME}_MAJOR_VERSION 1)

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING ${PROJECT_NAME}_MAJOR_VERSION
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION "${version}"
  COMPATIBILITY AnyNewerVersion
)

install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${PROJECT_NAME}/${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

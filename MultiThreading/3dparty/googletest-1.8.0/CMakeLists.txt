cmake_minimum_required(VERSION 3.3.2)

project(googletest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(googletest gmock-gtest-all.cc)

target_link_libraries(googletest pthread)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin
        ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
        LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)


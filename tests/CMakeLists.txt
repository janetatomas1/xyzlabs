
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(tests ${TEST_SOURCES})

target_include_directories(
    tests
    PRIVATE
    "${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_INCLUDE_PATH}"
)
target_link_directories(
    tests
    PRIVATE
    ${CMAKE_LIBRARY_PATH}
)
find_package(gtest REQUIRED)
target_link_libraries(
    tests
    PRIVATE
    xyzlabs
    gtest
)

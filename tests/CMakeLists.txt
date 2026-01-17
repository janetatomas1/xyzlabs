
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(tests ${TEST_SOURCES})

target_include_directories(
    tests
    PRIVATE
    "${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_INCLUDE_PATH}"
)

if(UNIX)
    set(GTEST_LIBRARY "libgtest.a")
else()
    set(GTEST_LIBRARY "gtest.lib")
endif()

find_package(GTest REQUIRED)
target_link_libraries(
    tests
    PRIVATE
    xyzlabs
    GTest::gtest
)

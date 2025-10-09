
cmake_minimum_required(VERSION 3.28)
project(xyzlabs)

if(DEFINED CMAKE_TOOLCHAIN_FILE_ AND EXISTS ${CMAKE_TOOLCHAIN_FILE_})
    include(${CMAKE_TOOLCHAIN_FILE_})
endif()

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options(/utf-8)
endif()

file(GLOB XYZLABS_SOURCES "${CMAKE_SOURCE_DIR}/src/xyzlabs/*.cpp")

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(fmt REQUIRED)
find_package(GLFW3 REQUIRED)

add_library(
    xyzlabs
    STATIC
    "${XYZLABS_SOURCES}"
)

target_compile_definitions(xyzlabs PUBLIC SPDLOG_FMT_EXTERNAL) 
target_compile_definitions(xyzlabs INTERFACE SPDLOG_FMT_EXTERNAL)

target_include_directories(
    xyzlabs
    PUBLIC
    "${CMAKE_INCLUDE_PATH}"
    "${CMAKE_SOURCE_DIR}/include/"
    "${IMGUI_DIR}"
    "${STB_DIR}"
    "${PFD_DIR}"
)
target_link_directories(
    xyzlabs
    PUBLIC
    ${CMAKE_LIBRARY_PATH}
)

if(MSVS) 
    set(GLEW_LIBRARY "libglew32")
    set(OPENGL_LIBRARY OpenGL::GL)
else()
    set(GLEW_LIBRARY GLEW)
    set(OPENGL_LIBRARY GL)
endif()
target_link_libraries(
    xyzlabs
    PUBLIC
    glfw3
    fmt
    spdlog
    ${OPENGL_LIBRARY}
    ${GLEW_LIBRARY}
    glfw3
    imgui
    implot
)

add_executable(
    burningforest
    ${CMAKE_SOURCE_DIR}/example/src/burningforest.cpp
    ${CMAKE_SOURCE_DIR}/example/src/main.cpp
)
target_include_directories(
    burningforest
    PRIVATE
    ${CMAKE_SOURCE_DIR}/example/include
    ${CMAKE_SOURCE_DIR}/include
)
target_link_directories(
    burningforest
    PUBLIC
    ${CMAKE_LIBRARY_PATH}
)
add_definitions(-DGLEW_STATIC)

target_link_libraries(
    burningforest
    PUBLIC
    xyzlabs
)
install(
    TARGETS xyzlabs
    ARCHIVE DESTINATION lib
)
install(
    DIRECTORY include/
    DESTINATION include
)
add_compile_options(
    -Wfatal-errors
)

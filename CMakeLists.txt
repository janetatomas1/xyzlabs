
cmake_minimum_required(VERSION 3.28)
project(xyzlabs)

if(DEFINED CMAKE_TOOLCHAIN_FILE_ AND EXISTS ${CMAKE_TOOLCHAIN_FILE_})
    include(${CMAKE_TOOLCHAIN_FILE_})
endif()

option(WITH_3D "Build xyzlabs3d library" ON)
option(WITH_EXAMPLE "Build example" ON)

set(CMAKE_CXX_STANDARD 23)

if(MSVC)
    add_compile_options(/utf-8)
endif()

file(GLOB XYZLABS_SOURCES "${CMAKE_SOURCE_DIR}/src/xyzlabs/*.cpp")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules" ${CMAKE_MODULE_PATH})
add_library(
    xyzlabs
    STATIC
    "${XYZLABS_SOURCES}"
)

find_package(GLFW REQUIRED)
find_package(fmt REQUIRED)
find_package(spdloG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui MODULE REQUIRED)

target_include_directories(
    xyzlabs
    PUBLIC
    ${CMAKE_INCLUDE_PATH}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_INCLUDE_PATH}>
    $<INSTALL_INTERFACE:include>
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
    fmt::fmt
    spdlog::spdlog
    ${OPENGL_LIBRARY}
    ${GLEW_LIBRARY}
    imgui::imgui
    GLFW::GLFW
)

add_definitions(-DGLEW_STATIC)
target_compile_definitions(xyzlabs PUBLIC SPDLOG_FMT_EXTERNAL)
target_compile_definitions(xyzlabs PUBLIC FMT_HEADER_ONLY)

if(WITH_3D)
    set(MAGNUM_APPLICATION_BACKEND GlfwApplication)

    find_package(
        Magnum
        REQUIRED
        GL
        SceneGraph
        ${MAGNUM_APPLICATION_BACKEND}
        Shaders
    )
    file(GLOB XYZLABS3D_SOURCES "${CMAKE_SOURCE_DIR}/src/xyzlabs3d/*.cpp")
    add_library(xyzlabs3d STATIC "${XYZLABS3D_SOURCES}")
    target_include_directories(
        xyzlabs3d
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )
    target_link_libraries(
        xyzlabs3d
        PUBLIC xyzlabs
        Magnum::GL
        Magnum::Application
        Magnum::Shaders
        Magnum::SceneGraph
    )
endif()

if(WITH_EXAMPLE AND NOT WITH_3D)
    message(FATAL_ERROR "xyzlabs3d library must be built for examples")
elseif(WITH_EXAMPLE)
    add_executable(triangle
        "${CMAKE_SOURCE_DIR}/src/triangle/main.cpp"
    )
    target_include_directories(
        triangle
        PRIVATE
        "${CMAKE_SOURCE_DIR}/include/"
    )
    target_link_libraries(
        triangle
        PRIVATE
        xyzlabs
        xyzlabs3d
    )
endif()

add_compile_options(
    -Wfatal-errors
)

install(
    TARGETS xyzlabs
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/xyzlabs
    DESTINATION include/
)

if(WITH_3D)
    install(
        TARGETS xyzlabs3d
        ARCHIVE DESTINATION lib
    )
    install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/include/xyzlabs3d
        DESTINATION include/
    )
endif()

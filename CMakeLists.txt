
cmake_minimum_required(VERSION 3.28)
project(xyzlabs)

if(DEFINED CMAKE_TOOLCHAIN_FILE_ AND EXISTS ${CMAKE_TOOLCHAIN_FILE_})
    include(${CMAKE_TOOLCHAIN_FILE_})
endif()

option(WITH_3D "Build xyzlabs3d library" ON)
option(WITH_EXAMPLES "Build example" ON)
option(WITH_TESTS "Build tests" ON)

set(CMAKE_CXX_STANDARD 23)

if(MSVC)
    add_compile_options(/utf-8)
endif()

file(GLOB_RECURSE XYZLABS_SOURCES "${CMAKE_SOURCE_DIR}/src/xyzlabs/*.cpp")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules" ${CMAKE_MODULE_PATH})

add_library(
    xyzlabs
    STATIC
    "${XYZLABS_SOURCES}"
)

find_package(glfw3 REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui REQUIRED)
find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(concurrentqueue REQUIRED)
find_package(stb REQUIRED)
find_package(GLEW REQUIRED)

target_include_directories(
    xyzlabs
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(MSVS)
    set(GLEW_LIBRARY "libglew32")
    set(OPENGL_LIBRARY OpenGL::GL)
else()
    set(GLEW_LIBRARY GLEW)
    set(OPENGL_LIBRARY GL)
endif()

set(CMAKE_VERBOSE_MAKEFILE 1)

target_link_libraries(
    xyzlabs
    PUBLIC
    fmt::fmt
    spdlog::spdlog
    OpenGL::GL
    GLEW::GLEW
    imgui::imgui
    glfw
    ${Boost_LIBRARIES}
    nlohmann_json::nlohmann_json
    concurrentqueue::concurrentqueue
    stb::stb
)
target_include_directories(
    xyzlabs
    PUBLIC
    ${CMAKE_INCLUDE_PATH}
)

add_definitions(-DGLEW_STATIC)
target_compile_definitions(xyzlabs PUBLIC SPDLOG_FMT_EXTERNAL)
target_compile_definitions(xyzlabs PUBLIC FMT_HEADER_ONLY)

get_target_property(LIBS imgui::imgui INTERFACE_INCLUDE_DIRECTORIES)

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
        GLEW::GLEW
    )
endif()

if(WITH_EXAMPLES AND NOT WITH_3D)
    message(FATAL_ERROR "xyzlabs3d library must be built for examples")
elseif(WITH_EXAMPLES)
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
        imgui::imgui
        xyzlabs3d
        GLEW::GLEW
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

add_compile_options(
    -Wfatal-errors -Wall -Wextra -Wpedantic
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

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

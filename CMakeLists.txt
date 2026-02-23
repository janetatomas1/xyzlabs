
cmake_minimum_required(VERSION 3.28)
project(xyzlabs)

if(DEFINED CMAKE_TOOLCHAIN_FILE AND EXISTS ${CMAKE_TOOLCHAIN_FILE})
    include(${CMAKE_TOOLCHAIN_FILE})
endif()

option(WITH_3D "Build xyzlabs3d library" OFF)
option(WITH_EXAMPLES "Build examples" OFF)
option(WITH_TESTS "Build tests" OFF)

if(MSVC)
else()
    add_compile_options(
        -Wfatal-errors -Wall -Wextra -Wpedantic
    )
endif()

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

find_package(glfw REQUIRED)
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

target_link_libraries(
    xyzlabs
    PUBLIC
    fmt::fmt
    spdlog::spdlog
    OpenGL::GL
    GLEW::GLEW
    imgui::imgui
    glfw::glfw
    ${Boost_LIBRARIES}
    nlohmann_json::nlohmann_json
    concurrentqueue::concurrentqueue
    stb::stb
)
target_compile_definitions(xyzlabs PUBLIC GLEW_STATIC)
target_compile_definitions(xyzlabs PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
target_compile_definitions(xyzlabs PUBLIC SPDLOG_FMT_EXTERNAL)
target_compile_definitions(xyzlabs PUBLIC FMT_HEADER_ONLY)

get_target_property(LIBS imgui::imgui INTERFACE_INCLUDE_DIRECTORIES)

if(WITH_3D)
    find_package(
        Magnum
        REQUIRED
        GL
        SceneGraph
        GlfwApplication
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

install(
    TARGETS xyzlabs
    EXPORT xyzlabsTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/xyzlabs
    DESTINATION include/
)

if(WITH_3D)
    install(
        TARGETS xyzlabs3d
        EXPORT xyzlabsTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
    )

    install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/include/xyzlabs3d
        DESTINATION include/
    )
endif()

install(
    EXPORT xyzlabsTargets
    FILE xyzlabsTargets.cmake
    NAMESPACE xyzlabs::
    DESTINATION lib/cmake/xyzlabs
)

if(WITH_EXAMPLES)
    add_subdirectory(examples)
endif()

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/xyzlabsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/xyzlabsConfig.cmake
    INSTALL_DESTINATION lib/cmake/xyzlabs
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/xyzlabsConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/xyzlabsConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/xyzlabsConfigVersion.cmake
    DESTINATION lib/cmake/xyzlabs
)

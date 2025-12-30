
cmake_minimum_required(VERSION 3.28)
project(xyzlabs)

if(DEFINED CMAKE_TOOLCHAIN_FILE_ AND EXISTS ${CMAKE_TOOLCHAIN_FILE_})
    include(${CMAKE_TOOLCHAIN_FILE_})
endif()

option(USE_GLFW "Enable GLFW backend" ON)
option(USE_SDL "Enable SDL backend" OFF)
option(WITH_3D "Build xyzlabs3d library" ON)
option(WITH_EXAMPLE "Build example" ON)

if(USE_GLFW)
    set(BACKEND_LIBRARY glfw3)
elseif(USE_SDL)
    set(BACKEND_LIBRARY SDL3)
endif()

set(CMAKE_CXX_STANDARD 23)

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
    imgui
    implot
)

add_definitions(-DGLEW_STATIC)

target_compile_definitions(xyzlabs PUBLIC SPDLOG_FMT_EXTERNAL)

if(USE_GLFW)
    target_compile_definitions(xyzlabs PUBLIC USE_GLFW)
    target_link_libraries(xyzlabs PUBLIC glfw3)
else()
    target_compile_definitions(xyzlabs PUBLIC USE_SDL)
    target_link_libraries(xyzlabs PUBLIC SDL3)
endif()

if(WITH_3D)
    find_package(
        Magnum
        REQUIRED
        GL
        GlfwApplication
        Shaders
    )
    file(GLOB XYZLABS3D_SOURCES "${CMAKE_SOURCE_DIR}/src/xyzlabs3d/*.cpp")
    add_library(xyzlabs3d STATIC "${XYZLABS3D_SOURCES}")
    target_include_directories(xyzlabs3d PUBLIC "${CMAKE_SOURCE_DIR}/include/")
    target_link_libraries(
        xyzlabs3d
        PUBLIC xyzlabs
        Magnum::GL
        Magnum::Application
        Magnum::Shaders
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

install(
    TARGETS xyzlabs
    ARCHIVE DESTINATION lib
)
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/xyzlabs
    DESTINATION include/xyzlabs
)
add_compile_options(
    -Wfatal-errors
)

if(WITH_3D)
    install(
        TARGETS xyzlabs3d
        ARCHIVE DESTINATION lib
    )
    install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/include/xyzlabs3d
        DESTINATION include/xyzlabs3d
    )
endif()

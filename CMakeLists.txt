
cmake_minimum_required(VERSION 3.28)
project(xyzlabs)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/third-party/gcc/conan_toolchain.cmake")
endif()

include(${CMAKE_TOOLCHAIN_FILE})

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/submodules/imgui/")
set(IMGUI_BACKENDS_DIR "${CMAKE_SOURCE_DIR}/submodules/imgui/backends")
set(IMPLOT_DIR "${CMAKE_SOURCE_DIR}/submodules/implot")
set(STB_DIR "${CMAKE_SOURCE_DIR}/submodules/stb")

set(
    IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp"
    "${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp"
)
set(
    IMPLOT_SOURCES
    "${IMPLOT_DIR}/implot.cpp"
    "${IMPLOT_DIR}/implot_items.cpp"
)

file(GLOB XYZLABS_SOURCES "${CMAKE_SOURCE_DIR}/src/xyzlabs/*.cpp")
add_executable(
    xyzlabs
    "${XYZLABS_SOURCES}"
    "${IMGUI_SOURCES}"
    "${IMPLOT_SOURCES}"
)

target_compile_definitions(xyzlabs PUBLIC SPDLOG_FMT_EXTERNAL) 
target_compile_definitions(xyzlabs INTERFACE SPDLOG_FMT_EXTERNAL)

target_include_directories(
    xyzlabs
    PRIVATE
    "${CMAKE_INCLUDE_PATH}"
    "${CMAKE_SOURCE_DIR}/include/xyzlabs"
    "${IMGUI_DIR}"
    "${IMGUI_BACKENDS_DIR}"
    "${IMPLOT_DIR}"
    "${STB_DIR}"
)
target_link_directories(
    xyzlabs
    PRIVATE
    ${CMAKE_LIBRARY_PATH}
)
target_link_libraries(
    xyzlabs
    PRIVATE
    glfw3
    spdlog
    fmt
)

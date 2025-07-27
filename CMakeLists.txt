
cmake_minimum_required(VERSION 3.28)
project(xyzlabs)

include(${CMAKE_TOOLCHAIN_FILE})

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/submodules/imgui/")
set(IMGUI_BACKENDS_DIR "${CMAKE_SOURCE_DIR}/submodules/imgui/backends")
set(IMPLOT_DIR "${CMAKE_SOURCE_DIR}/submodules/implot")
set(STB_DIR "${CMAKE_SOURCE_DIR}/submodules/stb")
set(PFD_DIR "${CMAKE_SOURCE_DIR}/submodules/portable-file-dialogs")
set(CONCURRENT_QUEUE_DIR "${CMAKE_SOURCE_DIR}/submodules/concurrentqueue/")
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
add_library(
    xyzlabs
    STATIC
    "${XYZLABS_SOURCES}"
    "${IMGUI_SOURCES}"
    "${IMPLOT_SOURCES}"
)

target_compile_definitions(xyzlabs PUBLIC SPDLOG_FMT_EXTERNAL) 
target_compile_definitions(xyzlabs INTERFACE SPDLOG_FMT_EXTERNAL)

target_include_directories(
    xyzlabs
    PUBLIC
    "${CMAKE_INCLUDE_PATH}"
    "${CMAKE_SOURCE_DIR}/include/"
    "${IMGUI_DIR}"
    "${IMGUI_BACKENDS_DIR}"
    "${IMPLOT_DIR}"
    "${STB_DIR}"
    "${PFD_DIR}"
    "${CONCURRENT_QUEUE_DIR}"
)
target_link_directories(
    xyzlabs
    PUBLIC
    ${CMAKE_LIBRARY_PATH}
)
target_link_libraries(
    xyzlabs
    PUBLIC
    glfw3
    spdlog
    GLEW
    fmt
    GL
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
target_link_libraries(
    burningforest
    PRIVATE
    xyzlabs
)

install()
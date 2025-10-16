
add_executable(
    burningforest
    ${CMAKE_CURRENT_SOURCE_DIR}/burningforest.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/burningforest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_include_directories(
    burningforest
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
target_link_directories(
    burningforest
    PUBLIC
    ${CMAKE_LIBRARY_PATH}
)

target_link_libraries(
    burningforest
    PUBLIC
    xyzlabs
)

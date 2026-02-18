
add_executable(
    triangle
    triangle/main.cpp
)
target_link_libraries(
    triangle
    PRIVATE
    xyzlabs
    xyzlabs3d
)

add_executable(
    helloworld
    helloworld/main.cpp
)
target_link_libraries(
    helloworld
    PRIVATE
    xyzlabs
)

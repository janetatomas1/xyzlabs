
include(FindPackageHandleStandardArgs)

if(UNIX)
    set(GLFW_LIBRARY_NAME "libglfw3.a")
elseif(WIN32)
    set(GLFW_LIBRARY_NAME "glfw.lib")
endif()

foreach(D ${CMAKE_LIBRARY_PATH})
    if(EXISTS "${D}")
        file(GLOB MATCHING_FILES "${D}/${GLFW_LIBRARY_NAME}")
        if(MATCHING_FILES)
            set(LIBRARY_FILE_FOUND TRUE)
            set(GLFW_LIBRARY "${D}/${GLFW_LIBRARY_NAME}")
            break()
        endif()
    endif()
endforeach()

foreach(D ${CMAKE_INCLUDE_PATH})
    if(EXISTS "${D}")
        file(GLOB MATCHING_FILES "${D}/GLFW/glfw3.h")
        if(MATCHING_FILES)
            set(HEADER_FILE_FOUND TRUE)
            set(GLFW_INCLUDE_DIR "${D}")
            break()
        endif()
    endif()
endforeach()


find_package_handle_standard_args(GLFW
    REQUIRED_VARS GLFW_LIBRARY GLFW_INCLUDE_DIR
)

if(IMGUI_FOUND AND NOT TARGET GLFW::GLFW)
    add_library(GLFW::GLFW UNKNOWN IMPORTED)

    set_target_properties(GLFW::GLFW PROPERTIES
        IMPORTED_LOCATION ${GLFW_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES "${IMGUI_INCLUDE_DIR}"
    )
endif()

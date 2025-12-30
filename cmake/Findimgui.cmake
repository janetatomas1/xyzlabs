
include(FindPackageHandleStandardArgs)

if(UNIX)
    set(IMGUI_LIBRARY_NAME "libimgui.a")
elseif(WIN32)
    set(IMGUI_LIBRARY_NAME "imgui.lib")
endif()

foreach(D ${CMAKE_LIBRARY_PATH})
    if(EXISTS "${D}")
        file(GLOB MATCHING_FILES "${D}/${IMGUI_LIBRARY_NAME}")
        if(MATCHING_FILES)
            set(LIBRARY_FILE_FOUND TRUE)
            set(IMGUI_LIBRARY "${D}/${IMGUI_LIBRARY_NAME}")
            break()
        endif()
    endif()
endforeach()

foreach(D ${CMAKE_INCLUDE_PATH})
    if(EXISTS "${D}")
        file(GLOB MATCHING_FILES "${D}/imgui.h")
        if(MATCHING_FILES)
            set(HEADER_FILE_FOUND TRUE)
            set(IMGUI_INCLUDE_DIR "${D}")
            break()
        endif()
    endif()
endforeach()


find_package_handle_standard_args(imgui
    REQUIRED_VARS IMGUI_LIBRARY IMGUI_INCLUDE_DIR
)

if(IMGUI_FOUND AND NOT TARGET imgui::imgui)
    add_library(imgui::imgui UNKNOWN IMPORTED)

    set_target_properties(imgui::imgui PROPERTIES
        IMPORTED_LOCATION "${IMGUI_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${IMGUI_INCLUDE_DIR}"
    )
endif()

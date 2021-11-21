if (POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif()
if (POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

set(CMAKE_MACOSX_RPATH 0)

add_subdirectory(core)
add_subdirectory(qt_storage)
add_subdirectory(qt_gui)
add_subdirectory(external_io)
add_subdirectory(main)

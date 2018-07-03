project(${STORAGE_LIB})

find_package(Qt5 REQUIRED COMPONENTS Core Sql)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the build dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${STORAGE_LIB} SHARED)

target_sources(${STORAGE_LIB}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage_impl/DBService.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage_impl/QtSprintDistributionReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage_impl/QtSprintStorageReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage_impl/QtSprintStorageWriter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage_impl/QtYearRangeReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage_impl/QtTaskStorageReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage_impl/QtTaskStorageWriter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage_impl/QtStorageImplementersFactory.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage_impl/Database.h"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/DBService.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtSprintDistributionReader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtSprintStorageReader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtSprintStorageWriter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtYearRangeReader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtTaskStorageReader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtTaskStorageWriter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/Database.cpp"
)

target_include_directories(${STORAGE_LIB} PUBLIC include PUBLIC src)

target_compile_features(${STORAGE_LIB} PRIVATE cxx_std_17)
target_compile_options(${STORAGE_LIB} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${STORAGE_LIB} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_link_libraries(${STORAGE_LIB}
    PUBLIC Qt5::Core
    PUBLIC Qt5::Sql
    PUBLIC ${CORE_LIB})

if (WIN32)
    install(TARGETS ${STORAGE_LIB} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
else()
    install(TARGETS ${STORAGE_LIB} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()

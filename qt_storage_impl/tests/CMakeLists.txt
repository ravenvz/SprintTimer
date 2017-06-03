cmake_minimum_required(VERSION 3.2)

set(PROJECT test_qt_storage_impl)
project(${PROJECT})
set(SOURCE_FILES
        test_qt_storage_impl.cpp
)

# this is required as headers and sources are in different directories
set(HEADER_FILES_FOR_AUTOMOC_TO_WORK
        ../include/qt_storage_impl/DBService.h
        ../include/qt_storage_impl/QtSprintDistributionReader.h
        ../include/qt_storage_impl/QtSprintStorageReader.h
        ../include/qt_storage_impl/QtSprintStorageWriter.h
        ../include/qt_storage_impl/QtYearRangeReader.h
        ../include/qt_storage_impl/QtTaskStorageWriter.h
)

add_executable(${PROJECT} ${SOURCE_FILES} ${HEADER_FILES_FOR_AUTOMOC_TO_WORK})
target_compile_features(${PROJECT} PRIVATE cxx_return_type_deduction)

target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_include_directories(${PROJECT}
    PUBLIC ../include
    PRIVATE ../src
)

target_link_libraries(${PROJECT} gtest gtest_main gmock ${STORAGE_LIB} Threads::Threads Qt5::Sql)

# TODO check if this is still relevant with GTest on Win box
# This is a workaround
# For some reason, although CppUTest explicitly depends on winmm, dependency does not transition.
if (WIN32)
    target_link_libraries(${PROJECT} winmm.lib)
endif()

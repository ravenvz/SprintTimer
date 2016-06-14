cmake_minimum_required(VERSION 3.2)

set(PROJECT test_qt_storage_impl)
project(${PROJECT})
set(SOURCE_FILES
    run_all_tests.cpp
    test_pomodoro_storage.cpp
    ../src/QtPomoDistributionReader.cpp
    ../src/QtPomoStorageReader.cpp
    ../src/QtPomoStorageWriter.cpp
    ../src/QtPomoYearRangeReader.cpp
    ../src/QtTaskStorageWriter.cpp
    ../src/db_service.cpp
)

# this is required as headers and sources are in different directories
set(HEADER_FILES_FOR_AUTOMOC_TO_WORK
    ../include/qt_storage_impl/db_service.h
    ../include/qt_storage_impl/QtPomoDistributionReader.h
    ../include/qt_storage_impl/QtPomoStorageReader.h
    ../include/qt_storage_impl/QtPomoStorageWriter.h
    ../include/qt_storage_impl/QtPomoYearRangeReader.h
    ../include/qt_storage_impl/QtTaskStorageWriter.h
)

add_executable(${PROJECT} ${SOURCE_FILES} ${HEADER_FILES_FOR_AUTOMOC_TO_WORK})
target_compile_features(${PROJECT} PRIVATE cxx_return_type_deduction)

# silence warnings from CppUTest
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(${PROJECT} PRIVATE ${CLANG_COMPILE_OPTIONS}
                                                     -Wno-keyword-macro
                                                     -Wno-inconsistent-missing-override
                                                     -Wno-reserved-id-macro
                                                     -Wno-disabled-macro-expansion
                                                     -Wno-old-style-cast)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT} PRIVATE ${GCC_COMPILE_OPTIONS}
                                                   -Wno-unused-parameter)
else()
  message(FATAL_ERROR "Unknown compiler.")
endif()

target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_include_directories(${PROJECT}
    PRIVATE /usr/include/CppUTest
    PUBLIC ../include
    PRIVATE ../src
)

target_link_libraries(${PROJECT} CppUTest Threads::Threads Qt5::Sql pomodoro_qt_common)

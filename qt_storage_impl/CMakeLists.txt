cmake_minimum_required(VERSION 3.2)

set(PROJECT ${STORAGE_LIB})
project(${PROJECT})

set(QT_STORAGE_IMPL_SOURCE_FILES
    src/db_service.cpp
        src/QtSprintDistributionReader.cpp
        src/QtSprintStorageReader.cpp
        src/QtSprintStorageWriter.cpp
        src/QtYearRangeReader.cpp
    src/QtTaskStorageReader.cpp
    src/QtTaskStorageWriter.cpp
        src/Database.cpp
)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the build dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# this is required as headers and sources are in different directories
set(HEADER_FILES_FOR_AUTOMOC_TO_WORK
        include/qt_storage_impl/db_service.h
        include/qt_storage_impl/QtSprintDistributionReader.h
        include/qt_storage_impl/QtSprintStorageReader.h
        include/qt_storage_impl/QtSprintStorageWriter.h
        include/qt_storage_impl/QtYearRangeReader.h
        include/qt_storage_impl/QtTaskStorageReader.h
        include/qt_storage_impl/QtTaskStorageWriter.h
        include/qt_storage_impl/QtStorageImplementersFactory.h
        include/qt_storage_impl/Database.h
)

set(THREADS_PREFER_PTHREAD_FLAG ON)

add_library(${PROJECT} SHARED ${QT_STORAGE_IMPL_SOURCE_FILES} ${HEADER_FILES_FOR_AUTOMOC_TO_WORK})

target_include_directories(${PROJECT} PUBLIC include PUBLIC src)
target_compile_features(${PROJECT} PRIVATE cxx_return_type_deduction)
target_link_libraries(${PROJECT}
    PUBLIC Qt5::Core
    PUBLIC Qt5::Sql
    PUBLIC ${CORE_LIB})

if (BUILD_TESTS)
    add_subdirectory(tests)
    # target_link_libraries(test_qt_storage_impl)
endif()

if (WIN32)
    install(TARGETS ${PROJECT} RUNTIME DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
else()
    install(TARGETS ${PROJECT} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()

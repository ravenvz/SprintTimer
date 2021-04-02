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
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/ConnectionGuard.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/DatabaseInitializer.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/DatabaseError.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/QtOperationalRangeReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/QtSprintDistributionReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/QtSprintStorageReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/QtSprintStorageWriter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/QtSprintStorage.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/QtStorageImplementersFactory.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/QtTaskStorage.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/QtTaskStorageReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/QtTaskStorageWriter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/QtWorkingDaysStorage.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/QueryError.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/TransactionGuard.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/WorkerConnection.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/migrations/Migration.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/migrations/MigrationManager.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/migrations/Migration_v3.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/migrations/Migration_v4.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/migrations/Migration_v5.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/migrations/Migration_v6.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/utils/DateTimeConverter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_storage/utils/QueryUtils.h"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/ConnectionGuard.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/DatabaseInitializer.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/DatabaseError.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtOperationalRangeReader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtSprintDistributionReader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtSprintStorageReader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtSprintStorageWriter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtSprintStorage.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtStorageImplementersFactory.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtTaskStorage.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtTaskStorageReader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtTaskStorageWriter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtWorkingDaysStorage.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QueryError.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/TransactionGuard.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/WorkerConnection.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/migrations/MigrationManager.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/migrations/Migration_v3.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/migrations/Migration_v4.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/migrations/Migration_v5.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/migrations/Migration_v6.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/utils/QueryUtils.cpp"
)

target_include_directories(${STORAGE_LIB} PUBLIC include PUBLIC src)

target_compile_features(${STORAGE_LIB} PRIVATE cxx_std_20)
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

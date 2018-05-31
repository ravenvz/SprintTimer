set(PROJECT ${CORE_LIB})
project(${PROJECT})
set(CORE_SOURCE_FILES
    src/SprintStatistics.cpp
    src/StatefulTimer.cpp
        src/CountdownTimer.cpp
    src/Sprint.cpp
    src/Task.cpp
    src/Tag.cpp
    src/StringUtils.cpp
    src/CoreService.cpp
    src/TagTop.cpp
    src/RegisterNewSprint.cpp
    src/RemoveSprintTransaction.cpp
    src/AddNewTask.cpp
    src/EditTask.cpp
    src/DeleteTask.cpp
    src/RequestFinishedTasks.cpp
    src/RequestSprints.cpp
    src/DecrementTaskSprints.cpp
    src/RenameTag.cpp
    src/IncrementTaskSprints.cpp
    src/RequestAllTags.cpp
    src/RequestSprintDistribution.cpp
    src/RequestUnfinishedTasks.cpp
    src/RequestTasks.cpp
    src/StoreUnfinishedTasksOrder.cpp
    src/ToggleTaskCompletionStatus.cpp
    src/CSVReader.cpp
    src/CSVEncoder.cpp
    src/SprintBuilder.cpp
    src/TaskBuilder.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost REQUIRED)

include(ExternalProject)
set(THIRD_PARTY_LIBS_FOLDER_PREFIX third_party)
include(CMakeLists-datelib.txt)

add_library(${PROJECT} SHARED ${CORE_SOURCE_FILES})

add_dependencies(${PROJECT} DateWrapper)

target_include_directories(${PROJECT}
    PUBLIC ${DateWrapper_INCLUDE_DIRS}
    PUBLIC "${date_INCLUDE_DIRS}" # workaround
    PUBLIC include
    PRIVATE src
    PUBLIC ${Boost_INCLUDE_DIRS})

target_compile_features(${PROJECT} PUBLIC cxx_std_17)

# TODO remove when CMake 3.10 is supported by MSCV
# Workaround until CMake would be able to pass cxx_std_17 to MSCV correctly
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++17" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++17")
    endif()
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(${PROJECT}
        INTERFACE DateWrapper
        PUBLIC stdc++fs
    )
elseif(WIN32)
    target_link_libraries(${PROJECT} INTERFACE DateWrapper)
endif()

if(BUILD_TESTS)
    include(CMakeLists-gtest.txt)
    add_subdirectory(tests)
endif()

if (WIN32)
    install(TARGETS ${PROJECT} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
else()
    install(TARGETS ${PROJECT} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()

cmake_minimum_required(VERSION 3.5)

set(PROJECT ${CORE_LIB})
project(${PROJECT})
set(CORE_SOURCE_FILES
    src/DateTime.cpp
    src/SprintStatistics.cpp
    src/StatefulTimer.cpp
    src/Timer.cpp
    src/TimeSpan.cpp
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
    src/StoreUnfinishedTasksOrder.cpp
    src/ToggleTaskCompletionStatus.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost REQUIRED)

include(ExternalProject)
set(THIRD_PARTY_LIBS_FOLDER_PREFIX third_party)
include(CMakeLists-datelib.txt)

add_library(${PROJECT} SHARED ${CORE_SOURCE_FILES})

add_dependencies(${PROJECT} DateLib)

target_include_directories(${PROJECT} PUBLIC ${Datelib_INCLUDE_DIRS} PUBLIC include PRIVATE src PUBLIC ${Boost_INCLUDE_DIRS})
target_compile_features(${PROJECT} PRIVATE cxx_return_type_deduction)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(${PROJECT} INTERFACE DateLib PUBLIC stdc++fs)
elseif(WIN32)
    target_link_libraries(${PROJECT} INTERFACE DateLib)
endif()

if(BUILD_TESTS)
    include(CMakeLists-cpputest.txt)
    add_subdirectory(tests)
endif()

if (WIN32)
   install(TARGETS ${PROJECT} RUNTIME DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
else()
   install(TARGETS ${PROJECT} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()

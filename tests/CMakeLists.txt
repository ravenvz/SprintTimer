cmake_minimum_required(VERSION 3.2)

set(TEST_PROJECT test_pomodoro)
project(${TEST_PROJECT})
set(SOURCE_FILES
    run_all_tests.cpp
    # ../src/db_layer/db_service.cpp
    # ../src/models/todoitemmodel.cpp
)
# find_library(CppUTest REQUIRED)
find_package(Qt5 REQUIRED Core Sql Widgets)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${TEST_PROJECT} ${SOURCE_FILES})
target_compile_features(${TEST_PROJECT} PRIVATE cxx_return_type_deduction)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
#   # silence warnings from CppUTest
    target_compile_options(${TEST_PROJECT} PRIVATE ${CLANG_COMPILE_OPTIONS}
                                                     -Wno-keyword-macro
                                                     -Wno-inconsistent-missing-override
                                                     -Wno-reserved-id-macro
                                                     -Wno-disabled-macro-expansion
                                                     -Wno-old-style-cast)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${TEST_PROJECT} PRIVATE ${GCC_COMPILE_OPTIONS}
                                                   -Wno-unused-parameter)
else()
  message(FATAL_ERROR "Unknown compiler.")
endif()

target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_include_directories(${TEST_PROJECT} PRIVATE /usr/include/CppUTest ../src lib)
target_link_libraries(${TEST_PROJECT} CppUTest Qt5::Core Qt5::Sql Qt5::Widgets Threads::Threads)

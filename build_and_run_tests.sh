#!/bin/bash

OPTIND=1

build_type="Debug"
c_compiler="gcc"
cxx_compiler="g++"
memory_sanitizer="OFF"
address_sanitizer="OFF"
ub_sanitizer="OFF"
thread_sanitizer="OFF"
make_tool="make"
make_options=-j$(nproc)
debug_make_options="-j1 VERBOSE=1"

while getopts "drcamutn" opt ; do
    case "$opt" in
        d) make_options=$debug_make_options
            ;;
        r) build_type="Release"
            ;;
        c) c_compiler="clang"
           cxx_compiler="clang++"
            ;;
        a) address_sanitizer="ON"
            ;;
        m) memory_sanitizer="ON"
           address_sanitizer="OFF"
            ;;
        u) ub_sanitizer="ON"
           memory_sanitizer="OFF"
           address_sanitizer="OFF"
           ;;
       t) thread_sanitizer="ON"
          memory_sanitizer="OFF"
          address_sanitizer="OFF"
          ub_sanitizer="OFF"
           ;;
       n) make_tool="ninja"
    esac
done

cmake_options="-DCMAKE_BUILD_TYPE=$build_type \
               -DADDRESS_SANITIZER=$address_sanitizer \
               -DMEMORY_SANITIZER=$memory_sanitizer \
               -DUB_SANITIZER=$ub_sanitizer \
               -DTHREAD_SANITIZER=$thread_sanitizer \
               -DBUILD_TESTS=ON"

if [[ $make_tool == 'ninja' ]]
then
    make_options=''
    cmake_options+=' -GNinja'
fi

echo "$build_type build using $cxx_compiler and $make_tool"

(cd build && \
    CC=$c_compiler CXX=$cxx_compiler cmake $cmake_options .. \
    && $make_tool $make_options)

(cd bin &&
    ./acceptance_tests_stub &&
    ./test_core &&
    ./test_qt_storage_impl)

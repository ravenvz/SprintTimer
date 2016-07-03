#!/bin/bash

OPTIND=1

build_type="Debug"
c_compiler="gcc"
cxx_compiler="g++"
memory_sanitizer="OFF"
address_sanitizer="OFF"
ub_sanitizer="OFF"
thread_sanitizer="OFF"
make_options=-j$(nproc)
debug_make_options="-j1 VERBOSE=1"

while getopts "drcamut" opt ; do
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
    esac
done

echo $make_options

echo "$build_type build using $cxx_compiler"

(cd build && CC=$c_compiler CXX=$cxx_compiler cmake \
    -DCMAKE_BUILD_TYPE=$build_type \
    -DADDRESS_SANITIZER=$address_sanitizer \
    -DMEMORY_SANITIZER=$memory_sanitizer \
    -DUB_SANITIZER=$ub_sanitizer \
    -DTHREAD_SANITIZER=$thread_sanitizer \
    .. && make $make_options)

(cd bin &&
    ./test_pomodoro &&
    ./test_pomodoro_core &&
    ./test_qt_storage_impl)

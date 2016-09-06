#!/bin/bash

OPTIND=1

build_type="Debug"
c_compiler="gcc"
cxx_compiler="g++"
sanitizers=""
make_tool="make"
build_tests="ON"
make_options=-j$(nproc)
debug_make_options="-j1 VERBOSE=1"

while getopts "drcamutnz" opt ; do
    case "$opt" in
        d) make_options=$debug_make_options
            ;;
        r) build_type="Release"
            ;;
        c) c_compiler="clang"
           cxx_compiler="clang++"
            ;;
        a) sanitizers+="address;"
            ;;
        m) sanitizers+="memory;"
            ;;
        u) sanitizers+="undefined;"
           ;;
        t) sanitizers+="thread;"
           ;;
        n) make_tool="ninja"
           ;;
        z) build_tests="OFF"
           ;;
    esac
done

cmake_options="-DCMAKE_BUILD_TYPE=$build_type \
               -DECM_ENABLE_SANITIZERS=$sanitizers \
               -DBUILD_TESTS=$build_tests"

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

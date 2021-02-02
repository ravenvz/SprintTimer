#!/bin/bash

# NOTE this is quick and dirty build script
# that is NOT intended to be used in automatic builds.
# In fact, it is intended to be used only on specific dev machine
set -e

OPTIND=1

build_type="Debug"
c_compiler="gcc"
cxx_compiler="g++"
sanitizers=""
build_tool="make"
build_tool_options=-j$(nproc)
build_tests="ON"
debug_make_options="-j1 VERBOSE=1"
verbose_makefile_option="OFF"
generator="Unix Makefiles"

while getopts "drcamutzvn" opt ; do
    case "$opt" in
        d) build_tool_options=$debug_make_options
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
        z) build_tests="OFF"
           ;;
		v) verbose_makefile_option="ON"
		   ;;
        n) build_tool="ninja"
           generator="Ninja"
           ;;
        *) exit 0
    esac
done

if [ -n "$sanitizers" ]; then
    echo "Tests could not be built with sanitizers' support yet"
    build_tests="OFF"
fi

cmake_options="-G $generator \
               -DCMAKE_BUILD_TYPE=$build_type \
               -DECM_ENABLE_SANITIZERS=$sanitizers \
               -DBUILD_TESTS=$build_tests \
               -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"


echo "$build_type build using $cxx_compiler and $build_tool"

(cd build && \
    CC=$c_compiler CXX=$cxx_compiler cmake -$cmake_options $verbose_makefile_option .. \
    && $build_tool $build_tool_options)

if [ $build_tests == "ON" ]; then
    (cd build && ctest --output-on-failure -j12)
fi

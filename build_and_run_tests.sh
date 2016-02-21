#!/bin/bash

OPTIND=1

build_type="Debug"

while getopts "r" opt ; do
    case "$opt" in
        r) build_type="Release"
    esac
done

echo $build_type

(cd build && cmake -DCMAKE_BUILD_TYPE=$build_type .. && scan-build make -j$(nproc))
(cd bin && ./test_pomodoro)

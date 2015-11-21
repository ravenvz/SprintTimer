#!/bin/bash

cd build && cmake .. && make -j$(nproc) && cd ../bin && ./test_pomodoro -c && cd ..

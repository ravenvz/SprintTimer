#!/bin/bash

cd build && cmake .. && make -j5 && cd ../bin && ./test_pomodoro -c && cd ..

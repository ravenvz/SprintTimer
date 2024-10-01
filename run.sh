# (cd build && CC=gcc CXX=g++ cmake -DENABLE_CACHE=OFF -DENABLE_TESTING=ON -DCMAKE_BUILD_TYPE=Debug -DWARNINGS_AS_ERRORS=False -DENABLE_SANITIZER_THREAD=False -DCMAKE_VERBOSE_MAKEFILE=OFF .. && cmake --build . --config=Debug -j 12)

(cd build && CC=gcc CXX=g++ cmake -G Ninja -DENABLE_CACHE=OFF -DENABLE_TESTING=ON -DCMAKE_BUILD_TYPE=Debug -DWARNINGS_AS_ERRORS=OFF -DENABLE_SANITIZER_THREAD=False -DCMAKE_VERBOSE_MAKEFILE=OFF .. && cmake --build . --config=Debug && ctest --progress --output-on-failure)

# (cd build && CC=gcc CXX=g++ cmake -DENABLE_CACHE=OFF -DENABLE_TESTING=ON -DCMAKE_BUILD_TYPE=Debug -DWARNINGS_AS_ERRORS=False -DENABLE_SANITIZER_THREAD=False -DCMAKE_VERBOSE_MAKEFILE=OFF .. && cmake --build . --config=Debug -j 1 && ctest --progress --output-on-failure)

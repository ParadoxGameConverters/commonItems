#!/bin/bash
cmake -H. -Bbuild && cmake --build ./build -- -j40
cd build
./CommonItemsTests
make gcov
make lcov
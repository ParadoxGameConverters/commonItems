#!/bin/bash
rm -rf build
cmake -H. -Bbuild && cmake --build ./build --target CommonItemsTests -- -j40
cd build
./CommonItemsTests
make gcov
make lcov
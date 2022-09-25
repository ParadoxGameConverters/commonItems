#!/bin/bash
cmake -H. -Bbuild && cmake --build --target CommonItemsTests ./build -- -j40
cd build
./CommonItemsTests
make gcov
make lcov
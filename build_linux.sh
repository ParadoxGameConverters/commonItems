#!/bin/sh
# shellcheck disable=SC2103

export CC=/usr/bin/gcc-11 &&
export CXX=/usr/bin/g++-11 &&

cmake --preset x64-release-linux
cmake --build --preset build-x64-release-linux --target CommonItemsTests -- -j40

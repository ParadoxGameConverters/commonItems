#!/bin/sh
# shellcheck disable=SC2103

export CC=/usr/bin/gcc-11 &&
export CXX=/usr/bin/g++-11 &&

cmake -H. -Bbuild &&
cmake --build build -- -j40

#!/bin/sh
# shellcheck disable=SC2103

export CC=/usr/bin/gcc-10 &&
export CXX=/usr/bin/g++-10 &&

cmake -H. -Bbuild &&
cmake --build build -- -j3
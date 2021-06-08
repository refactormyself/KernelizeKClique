#!/usr/bin/env bash
set -e

cd ../eclipse-build
#rm -r .
cmake -G "Eclipse CDT4 - Unix Makefiles"  \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE \
    -DCMAKE_ECLIPSE_MAKE_ARGUMENTS=-j3 \
    ../
#    -DCMAKE_ECLIPSE_VERSION=4.13 \

cd ../codeblocks-build
#rm -r .
cmake .. -G "CodeBlocks - Unix Makefiles"
cd ..